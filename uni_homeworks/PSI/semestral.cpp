/*
Testování:
server -> ./a.out 3999
test image -> tester 3999 gw
+ wireshark
*/

/*
1, Ideální situace
2, Autentizace
3, Segmentace a spojování
4, Timeout
5, Pohyb sondy/robota

BONUSY:
6, Syntaktické chyby
7, Optimalizace
8, Dobíjení
9, Vícevláknovost
10, Náhodné hodnoty
11, Finální test
*/

/*  POSTUP:
AUTENTIZACE: - sekvence kroků
Robot odešle své uživatelské jméno (CLIENT_USERNAME)
Server vyzve klienta k odeslání keyID (SERVER_KEY_REQUEST)
Klient odešle serveru keyID (CLIENT_KEY_ID)
Poté server zná správnou dvojici klíčů a spočítá hash
Server pošle klientovi potvrzovací kód serveru (SERVER_CONFIRMATION) a klient ho zkontroluje
Klient odešle serveru potvrzovací kód klienta (CLIENT_CONFIRMATION)
Server z potvrzovacího kódu klienta vypočte hash
    a po kontrole ho potvrdí (SERVER_OK) nebo zamítne (SERVER_LOGIN_FAILED) a ukončí spojení

Poté server pohne s robotem, aby zjistil jeho aktuální pozici. (dvakrát move aby zjistil i směr)
Poté server robota navádí do pozice 0,0

Po dosažení pozice 0,0 server zažádá o vyzvednutí tajného vzkazu (SERVER_PICK_UP)
Robot reaguje na vyzvednutí (CLIENT_MESSAGE)
Server zprávu potvrdí (SERVER_LOGOUT)

VÝPOČET HASH:
Uživatelské jméno: Mnau!
-> ASCII reprezentace: 77 110 97 117 33
-> Vzoreček pro hash: ((77 + 110 + 97 + 117 + 33) * 1000) % 65536 = 40784 (16bitové číslo v decimální podobě)
-> přičtení SERVER/KLIENT KEY: (40784 + 23019) % 65536 = 63803 (klidně přetečení)

POHYB ROBOTA K CÍLI: - stavový automat
Robot dostává příkazy:
    k pohybu vpřed (SERVER_MOVE)
    otočení doleva (SERVER_TURN_RIGHT)
    otočení doprava (SERVER_TURN_LEFT)
a po každém odešle potvrzení a aktuální pozici (CLIENT_OK)
- omezený počet pohybů vpřed
- omezený počet naražení do překážky (20)

Překážky:
- vždy jen jedna souřadnice obklopená volnými souřadnicemi
- cíl 0,0 nemůže být překážka
- detekována tak, že po pohybu vpřed nedošlo ke změně souřadnice
- pokud do ní narazí, nesníží se počet zbývajících pohybů vpřed

DOBÍJENÍ:
- pokud robotovi dochází energie, oznámí to serveru (CLIENT_RECHARGING)
- během dobíjení nereaguje na žádné zprávy
- po dobití (CLIENT_FULL_POWER) pokračuje ve stavu před dobíjením
- pokud dobíjení neskončí během TIMEOUT_RECHARGING, server ukončí spojení

CHYBOVÉ SITUACE:
- Pokud je ve zprávě klienta (CLIENT_KEY_ID)
    keyID mimo rozsah 0-4
    -> server reaguje (SERVER_KEY_OUT_OF_RANGE_ERROR) a ukončí spojení
    nečíselná hodnota
    -> server reaguje (SERVER_SYNTAX_ERROR) a ukončí spojení
- Pokud je ve zprávě klienta (CLIENT_CONFIRMATION)
    neodpovídající číslo
    -> server reaguje (SERVER_LOGIN_FAILED) a ukončí spojení
    nečíselná hodnota
    -> server reaguje (SERVER_SYNTAX_ERROR) a ukončí spojení
- Pokud je zpráva syntakticky špatně
    delší zpráva než definovaný počet znaků (včetně '\a\b')
    jiná zpráva než definované pro klienta
    -> server reaguje (SERVER_SYNTAX_ERROR) a ukončí spojení
- Pokud nastane logická chyba
    po zprávě o dobíjení (CLIENT_RECHARGING) pošle jinou zprávu než potvrzení o dobití (CLIENT_FULL_POWER)
    nebo pokud potvrdí dobití (CLIENT_FULL_POWER) bez předchozího dobíjení (CLIENT_RECHARGING)
    -> server reaguje (SERVER_LOGIC_ERROR) a ukončí spojení

TIMEOUT
- pokud během této doby nedostane server nebo klient odpověď, spojení se ukončí
TIMEOUT_RECHARGING
- pokud během této doby nezareaguje robot na dobíjení, spojení se ukončí

SPECIÁLNÍ SITUACE:
- při čtení ze socketu mohou být zprávy rozdělené, nebo více zpráv připraveno ke čtení najednou

OPTIMALIZACE:
- pokud zpráva přesahuje povolenou délku ještě před sekvencí '\a\b'
    -> tak server rovnou reaguje chybovou zprávou (SERVER_SYNTAX_ERROR) a ukončí spojení
- pokud během navádění k cíli robot odešle zprávu delší než očekávané zprávy
    -> tak server rovnou reaguje chybovou zprávou (SERVER_SYNTAX_ERROR) a ukončí spojení
*/

/*
POZNÁMKY:
zprávy končí \a\b
*/

/*    ZPRÁVY SERVERU:
Název	            Zpráva	                                    Popis
SERVER_CONFIRMATION	            <16-bitové číslo v decimální notaci>\a\b	Zpráva s potvrzovacím kódem. Může obsahovat maximálně 5 čísel a ukončovací sekvenci \a\b.
SERVER_MOVE            	        102 MOVE\a\b	                            Příkaz pro pohyb o jedno pole vpřed
SERVER_TURN_LEFT	            103 TURN LEFT\a\b	                        Příkaz pro otočení doleva
SERVER_TURN_RIGHT	            104 TURN RIGHT\a\b	                        Příkaz pro otočení doprava
SERVER_PICK_UP	                105 GET MESSAGE\a\b	                        Příkaz pro vyzvednutí zprávy
SERVER_LOGOUT	                106 LOGOUT\a\b	                            Příkaz pro ukončení spojení po úspěšném vyzvednutí zprávy
SERVER_KEY_REQUEST	            107 KEY REQUEST\a\b	                        Žádost serveru o Key ID pro komunikaci
SERVER_OK	                    200 OK\a\b	                                Kladné potvrzení
SERVER_LOGIN_FAILED	            300 LOGIN FAILED\a\b	                    Nezdařená autentizace
SERVER_SYNTAX_ERROR	            301 SYNTAX ERROR\a\b	                    Chybná syntaxe zprávy
SERVER_LOGIC_ERROR	            302 LOGIC ERROR\a\b	                        Zpráva odeslaná ve špatné situaci
SERVER_KEY_OUT_OF_RANGE_ERROR	303 KEY OUT OF RANGE\a\b	                Key ID není v očekávaném rozsahu
*/

/*     ZPRÁVY KLIENTA:
Název	              Zpráva	                                  Popis	                                                                                                        Ukázka	        Maximální délka
CLIENT_USERNAME	      <user name>\a\b	                          Zpráva s uživatelským jménem. Jméno může být libovolná sekvence 18 znaků kromě kromě dvojice \a\b.	        Umpa_Lumpa\a\b	20
CLIENT_KEY_ID	      <Key ID>\a\b	                              Zpráva obsahující Key ID. Může obsahovat pouze celé číslo o maximálně třech cifrách.	                        2\a\b	        5
CLIENT_CONFIRMATION	  <16-bitové číslo v decimální notaci>\a\b	  Zpráva s potvrzovacím kódem. Může obsahovat maximálně 5 čísel a ukončovací sekvenci \a\b.	                    1009\a\b	    7
CLIENT_OK	          OK <x> <y>\a\b	                          Potvrzení o provedení pohybu, kde x a y jsou celočíselné souřadnice robota po provedení pohybového příkazu.	OK -3 -1\a\b	12
CLIENT_RECHARGING	  RECHARGING\a\b	                          Robot se začal dobíjet a přestal reagovat na zprávy.		                                                                    12
CLIENT_FULL_POWER	  FULL POWER\a\b	                          Robot doplnil energii a opět příjímá příkazy.		                                                                            12
CLIENT_MESSAGE	      <text>\a\b	                              Text vyzvednutého tajného vzkazu. Může obsahovat jakékoliv znaky kromě ukončovací sekvence \a\b.	            Haf!\a\b        100
*/


/*      ČASOVÉ KONSTANTY:
Název	            Hodnota [s]	    Popis
TIMEOUT	            1	            Server i klient očekávají od protistrany odpověď po dobu tohoto intervalu.
TIMEOUT_RECHARGING	5	            Časový interval, během kterého musí robot dokončit dobíjení.
*/

/*      AUTENTIZACE:
Key ID	    Server Key 	    Client Key
0	        23019	        32037
1	        32037	        29295
2    	    18789	        13603
3        	16443	        29533
4	        18189	        21952
*/

/*
s = socket() - vytvoření prázdného socketu
bind(s) - připojení socketu ke konkrétnímu rozhraní na základě adresy
listen(s) - určení socketu ke spojení a nastaví maximum možných spojení

client = accept(s) - blokující funkce (čeká na příchozí spojení), po přijetí žádosti od klienta je vytvořen nový deskriptor, který je určený konkrétnímu klientovi (máme adresu a port klienta), původní pasivní deksriptor beze změn
select(client) - umožnuje čekat na více socketů
recv(client) - blokující funkce na čtení dat z bufferu, kam se ukládá komunikace od klienta
send(client) - blokující funkce na odeslání bufferu klientovi

close(s)
close(client) - uvolnění prostředků a ukončení komunikace
*/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <string.h>
#include <wait.h> // waitpid()
#include <map>
#include <string>
#include <sstream>
#include <cmath>

const int SERVER_KEYS [5] =
{
    23019, 32037, 18789, 16443, 18189
};

const int KLIENT_KEYS [5] =
{
    32037, 29295, 13603, 29533, 21952
};

const char END_OF_MESSAGE [3] = "\a\b";

const int TIMEOUT = 1;
const int TIMEOUT_RECHARGING = 5;
const int ERROR_VALUE = -1;
const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 100 + 1;
const int USERNAME_SIZE = 18;
const int MAX_SERVER_MESSAGE = 30;

const char SERVER_MOVE [ MAX_SERVER_MESSAGE ] = "102 MOVE\a\b";
const char SERVER_TURN_LEFT [ MAX_SERVER_MESSAGE ] = "103 TURN LEFT\a\b";
const char SERVER_TURN_RIGHT [ MAX_SERVER_MESSAGE ] = "104 TURN RIGHT\a\b";
const char SERVER_PICK_UP [ MAX_SERVER_MESSAGE ] = "105 GET MESSAGE\a\b";
const char SERVER_LOGOUT [ MAX_SERVER_MESSAGE ] = "106 LOGOUT\a\b";
const char SERVER_KEY_REQUEST [ MAX_SERVER_MESSAGE ] = "107 KEY REQUEST\a\b";
const char SERVER_OK [ MAX_SERVER_MESSAGE ] = "200 OK\a\b";
const char SERVER_LOGIN_FAILED [ MAX_SERVER_MESSAGE ] = "300 LOGIN FAILED\a\b";
const char SERVER_SYNTAX_ERROR [ MAX_SERVER_MESSAGE ] = "301 SYNTAX ERROR\a\b";
const char SERVER_LOGIC_ERROR [ MAX_SERVER_MESSAGE ] = "302 LOGIC ERROR\a\b";
const char SERVER_KEY_OUT_OF_RANGE_ERROR [ MAX_SERVER_MESSAGE ] = "303 KEY OUT OF RANGE\a\b";


const std::size_t CLIENT_USERNAME = 18;
const std::size_t CLIENT_KEY_ID = 3;
const std::size_t CLIENT_CONFIRMATION = 5;
const std::size_t CLIENT_OK = 10;
const std::size_t CLIENT_MESSAGE = 98;
const std::size_t CLIENT_RECHARGING_SIZE = 10;
const char CLIENT_RECHARGING [11] = "RECHARGING";
const char CLIENT_RECHARGING_BUFFER [13] = "RECHARGING\a\b";
const char CLIENT_FULL_POWER [11] = "FULL POWER";


// char CLIENT_USERNAME [18];
// char CLIENT_KEY_ID [3];
// char CLIENT_CONFIRMATION [5];
// char CLIENT_OK [10];
// char CLIENT_RECHARGING [11] = "RECHARGING";
// char CLIENT_FULL_POWER [11] = "FULL POWER";
// char CLIENT_MESSAGE [98];


struct TCoordinates
{
    int m_X;
    int m_Y;

    TCoordinates () {}
    TCoordinates ( const int x, const int y )
    : m_X (x), m_Y(y) {}
    
    friend bool operator != ( const TCoordinates& x, const TCoordinates& y );
    friend bool operator == ( const TCoordinates& x, const TCoordinates& y );
    friend TCoordinates& operator += ( TCoordinates& x, const TCoordinates& y );
    friend bool operator < ( const TCoordinates& x, const TCoordinates& y );
};

bool operator < ( const TCoordinates& x, const TCoordinates& y )
{
    return abs (x . m_X) + abs (x . m_Y) < abs (y . m_X) + abs (y . m_Y);
}

TCoordinates& operator += ( TCoordinates& x, const TCoordinates& y )
{
    x . m_X += y . m_X;
    x . m_Y += y . m_Y;
    return x;
}

bool operator != ( const TCoordinates& x, const TCoordinates& y )
{
    return x . m_X != y . m_X || x . m_Y != y . m_Y;
}

bool operator == ( const TCoordinates& x, const TCoordinates& y )
{
    return x . m_X == y . m_X && x . m_Y == y . m_Y;
}

TCoordinates FINISH = { 0, 0 };

int argCountError()
{
    std::cout << "Aplikace potřebuje zadat port!" << std::endl;
    return ERROR_VALUE;
}

bool portError()
{
    std::cout << "Chyba při získávání portu" << std::endl;
    return false;
}

bool endPointError()
{
    perror("Chyba při vytváření socketu: ");
    return false;
}

bool bindSocketError ( int& server )
{
    perror("Chyba při přiřazení socketu rozhraní: ");
    close ( server );
    return false;
}

bool listenSocketError ( int& server )
{
    perror("Chyba při nastavení socketu jako pasivní: ");
    close ( server );
    return false;
}

bool acceptConnectionError ( int& server )
{
    perror ( "Chyba při připojování klienta:" );
    close ( server );
    return false;
}

bool createEndPoint ( int& server )
{
    server = socket(AF_INET, SOCK_STREAM, 0);
    return server >= 0;
}

void createAddress ( sockaddr_in& address, int& port )
{
    bzero ( &address, sizeof ( address ) );
    address . sin_family = AF_INET;
    address . sin_port = htons ( port );
    address . sin_addr . s_addr = htonl ( INADDR_ANY );
}

bool bindSocket ( int& server, sockaddr_in& address )
{
    return bind ( server, ( struct sockaddr* ) &address, sizeof ( address ) ) >= 0;
}

bool listenSocket ( int& server )
{
    return listen ( server, MAX_CLIENTS ) >= 0;
}

bool setUpServer ( int& port, int& server, sockaddr_in& address )
{
    if ( port == 0 )
        return portError();

    if ( ! createEndPoint ( server ) )
        return endPointError();

    createAddress ( address, port );

    if ( ! bindSocket ( server, address ) )
        return bindSocketError ( server );

    if ( ! listenSocket ( server ) )
        return listenSocketError ( server );

    return true;
}

bool connectToClient ( int& server, int& client, sockaddr_in& remote_address, socklen_t& remote_address_len )
{
    client = accept ( server, (struct sockaddr *) &remote_address, &remote_address_len);
    if ( client < 0 )
        return acceptConnectionError ( server );
    return true;
}

bool notSubprocess ( pid_t& pid, int& client )
{
    if ( pid != 0 )
    {
        int status = 0;
        waitpid ( 0, &status, WNOHANG );
        close ( client );
        return true;
    }
    return false;
}

class CClientCommunication
{
  public:
    // Constructor, ...
    CClientCommunication ( int client_socket )
    : client ( client_socket ) {}

    CClientCommunication()
    {
        close ( client );
    }

    // Methods
    bool solve()
    {
        if ( ! authenticate() )
            return false;

        if ( ! treasureHunt() )
            return false;
        return true;
    }
    // Variables

  private:
    // Methods
    
    /** -------------------------
      * -      AUTENTIZACE      -
      * ------------------------- */

    bool authenticate()
    {
        // Robot odešle své uživatelské jméno (CLIENT_USERNAME)
        if ( ! readMessage ( CLIENT_USERNAME ) )
            return false;
        int username = getUsername ();

        // Server vyzve klienta k odeslání keyID (SERVER_KEY_REQUEST)
        if ( ! sendMessage ( SERVER_KEY_REQUEST ) )
            return false;

        // Klient odešle serveru keyID (CLIENT_KEY_ID)
        if ( ! readMessage ( CLIENT_RECHARGING_SIZE ) )
            return false;
        
        int keyID;
        if ( ! checkKeyID ( keyID ) )
            return false;

        // Poté server zná správnou dvojici klíčů a spočítá hash
        std::string hash_message = std::to_string ( getHash ( username, keyID ) );
        hash_message += END_OF_MESSAGE;

        // Server pošle klientovi potvrzovací kód serveru (SERVER_CONFIRMATION) a klient ho zkontroluje
        if ( ! sendMessage ( hash_message . c_str() ) )
            return false;

        // Klient odešle serveru potvrzovací kód klienta (CLIENT_CONFIRMATION)
        if ( ! readMessage ( CLIENT_RECHARGING_SIZE ) )
            return false;
        
        // Server z potvrzovacího kódu klienta vypočte hash
        //     a po kontrole ho potvrdí (SERVER_OK) nebo zamítne (SERVER_LOGIN_FAILED) a ukončí spojení
        if ( ! checkHash ( username, keyID ) )
            return false;

        if ( ! sendMessage ( SERVER_OK ) )
            return false;

        return true;
    }

    bool checkHash ( int hash, int keyID )
    {
        int hash_code;
        if ( current_message . size() > CLIENT_CONFIRMATION
            || ! strToDec ( hash_code ) )
        {
            sendMessage ( SERVER_SYNTAX_ERROR );
            return false;
        }
        hash_code -= KLIENT_KEYS [ keyID ];
        if ( hash_code < 0 )
            hash_code = hash_code + 65536;

        if ( hash != hash_code )
        {
            sendMessage ( SERVER_LOGIN_FAILED );
            return false;
        }
        return true;
    }

    bool checkKeyID ( int& keyID )
    {
        if ( current_message . size() > CLIENT_KEY_ID
            || ! strToDec ( keyID ) )
        {
            sendMessage ( SERVER_SYNTAX_ERROR );
            return false;
        }
        if ( keyID < 0 || keyID > 4 )
        {
            sendMessage ( SERVER_KEY_OUT_OF_RANGE_ERROR );
            return false;
        }
        return true;
    }

    bool strToDec ( int& num )
    {
        const char * tmp = current_message . c_str();
        for ( size_t i = 0; i < current_message . size(); i ++ )
        {
            if ( ! isdigit ( tmp [i] ) )
                return false;
        }
        
        num = std::stoi ( current_message );
        return true;
    }

    int getHash ( int username, int keyID )
    {
        return ( username + SERVER_KEYS [keyID] ) % 65536;
    }

    int getUsername ()
    {
        int username = 0;
        const char * tmp = current_message . c_str();
        for ( size_t i = 0; i < current_message . size(); i ++ )
            username += tmp [i];
        username *= 1000;
        username %= 65536;
        return username;
    }

    /** ---------------------------------
      * -      POHYB ROBOTA K CÍLI      -
      * --------------------------------- */

    bool treasureHunt()
    {
        bool wall;
        // Poté server pohne s robotem, aby zjistil jeho aktuální pozici. (dvakrát move aby zjistil i směr)
        if ( ! initMove ( wall ) )
            return false;

        // Poté server robota navádí do pozice 0,0
        while ( curr_pos != FINISH )
        {
            if ( ! choosePath ( wall ) )
                return false;

            if ( curr_pos == FINISH )
                break;

            if ( wall )
            {
                wall = false;
                if ( ! choosePathWall ( wall ) )
                    return false;
            }
        }

        // Po dosažení pozice 0,0 server zažádá o vyzvednutí tajného vzkazu (SERVER_PICK_UP)
        if ( ! sendMessage ( SERVER_PICK_UP ) )
            return false;

        // Robot reaguje na vyzvednutí (CLIENT_MESSAGE)
        if ( ! readMessage ( CLIENT_MESSAGE ) )
            return false;

        // Server zprávu potvrdí (SERVER_LOGOUT)
        if ( ! sendMessage ( SERVER_LOGOUT ) )
            return false;

        return true;
    }

    bool choosePathWall ( bool& wall )
    {
        TCoordinates left, right;
        getCoordinatesWall ( left, right );
        if ( left < curr_pos )
        {
            if ( ! fullMove ( SERVER_TURN_LEFT, wall ) )
                return false;
            if ( curr_pos == FINISH )
                return true;
            if ( ! fullMove ( SERVER_TURN_RIGHT, wall ) )
                return false;
            return true;
        }
        if ( ! fullMove ( SERVER_TURN_RIGHT, wall ) )
            return false;
        if ( curr_pos == FINISH )
            return true;
        if ( ! fullMove ( SERVER_TURN_LEFT, wall ) )
            return false;

        return true;
    }

    bool choosePath ( bool& wall )
    {
        TCoordinates left, right;
        getCoordinates ( left, right );
        if ( direction < curr_pos )
        {
            if ( ! fullMove ( SERVER_MOVE, wall ) )
                return false;
            return true;
        }
        if ( right < curr_pos )
        {
            if ( ! fullMove ( SERVER_TURN_RIGHT, wall ) )
                return false;
            return true;
        }
        if ( ! fullMove ( SERVER_TURN_LEFT, wall ) )
            return false;
        return true;
    }
    
    void getCoordinates ( TCoordinates& left, TCoordinates& right )
    {
        if ( direction != TCoordinates ( 0, 1 )
            && direction != TCoordinates ( 0, -1 ) )
        {
            left = TCoordinates ( direction . m_Y, direction . m_X );
            right = TCoordinates ( - direction . m_Y, - direction . m_X );
        } else {
            left = TCoordinates ( - direction . m_Y, - direction . m_X );
            right = TCoordinates ( direction . m_Y, direction . m_X );
        }
        left += curr_pos;
        right += curr_pos;
        direction += curr_pos;
    }

    void getCoordinatesWall ( TCoordinates& left, TCoordinates& right )
    {
        if ( direction != TCoordinates ( 0, 1 )
            && direction != TCoordinates ( 0, -1 ) )
        {
            left = TCoordinates ( direction . m_X, direction . m_X );
            right = TCoordinates ( direction . m_X, - direction . m_X );
        } else {
            left = TCoordinates ( - direction . m_Y, direction . m_Y );
            right = TCoordinates ( direction . m_Y, direction . m_Y );
        }
        left += curr_pos;
        right += curr_pos;
    }

    bool initMove ( bool& wall )
    {
        if ( ! fullMove ( SERVER_TURN_RIGHT, wall ) )
            return false;

        if ( ! getDirection() )
        {
            if ( ! fullMove ( SERVER_TURN_RIGHT, wall ) )
                return false;
        }
        return getDirection();
    }

    bool fullMove ( const char * move_type, bool& wall )
    {
        if ( move_type != SERVER_MOVE )
        {
            if ( ! move ( move_type ) )
                return false;
        }        
        if ( ! move ( SERVER_MOVE ) )
            return false;

        wall = ! getDirection();
        return true;
    }

    bool move ( const char * move_type )
    {
        if ( ! sendMessage ( move_type ) )
            return false;

        if ( ! readMessage ( CLIENT_OK ) )
            return false;

        if ( ! extractPos () )
            return false;
        return true;
    }

    bool extractPos()
    {
        std::istringstream iss ( current_message );
        iss >> std::noskipws;

        int x, y;
        char ok [3];
        char space1, space2;
        iss >> ok >> space1 >> x >> space2 >> y;
        if ( ! strcmp ( ok, "OK" ) && space1 == ' ' && space2 == ' ' && iss . eof() )
        {
            prev_pos = curr_pos;
            curr_pos = TCoordinates ( x, y );
            return true;
        }
        sendMessage ( SERVER_SYNTAX_ERROR );
        return false;
    }

    bool getDirection ()
    {
        direction . m_X = curr_pos . m_X - prev_pos . m_X;
        direction . m_Y = curr_pos . m_Y - prev_pos . m_Y;
        if ( direction . m_X == direction . m_Y )
            return false;
        return true;
    }

    /** -------------------------------
      * -      READING / SENDING      -
      * ------------------------------- */

    bool sendMessage ( const char* message )
    {
        if ( send ( client, message, strlen ( message ), MSG_NOSIGNAL ) < 0 )
        {
            perror ( "Chyba při odesílání ze socketu:" );
            return false;
        }
        std::cout << "[SENDING] (size=" << strlen ( message ) << "): " << message << std::endl;
        return true;
    }

    bool readMessage ( std::size_t max_size )
    {
        while ( ! messageReady() )
        {
            if ( prev_buffer . size() > max_size ) // optimalization
            {
                sendMessage ( SERVER_SYNTAX_ERROR );
                return false;
            }
            if ( ! fillBuffer ( false ) )
                return false;
        }
        if ( current_message == CLIENT_RECHARGING )
        {
            if ( ! recharge() )
                return false;
            return readMessage ( max_size );
        }
        std::size_t pos;
        pos = prev_buffer . find ( CLIENT_RECHARGING_BUFFER );
        if ( pos != std::string::npos )
        {
            std::string tmp = current_message;
            prev_buffer . erase ( pos, 2 + CLIENT_RECHARGING_SIZE );
            if ( ! recharge() )
                return false;
            current_message = tmp;
        }
        if ( current_message . size() > max_size )
        {
            sendMessage ( SERVER_SYNTAX_ERROR );
            return false;
        }
        return true;
    }

    bool recharge()
    {
        if ( ! fillBuffer ( true ) )
            return false;
        while ( ! messageReady() )
        {
            if ( prev_buffer . size() > CLIENT_RECHARGING_SIZE ) // optimalization
            {
                sendMessage ( SERVER_LOGIC_ERROR );
                return false;
            }
            if ( ! fillBuffer ( false ) )
                return false;
        }
        if ( current_message != CLIENT_FULL_POWER )
        {
            sendMessage ( SERVER_LOGIC_ERROR );
            return false;
        }
        return true;
    }

    bool messageReady ()
    {
        std::size_t pos = prev_buffer . find ( END_OF_MESSAGE );
        if ( pos != std::string::npos ) // found
        {
            current_message = prev_buffer . substr ( 0, pos );
            prev_buffer = prev_buffer . substr ( pos + 2 );
            return true;
        }
        return false;
    }

    bool fillBuffer ( bool recharging )
    {
        if ( ! waitForMessage ( recharging ) )
            return false;
        bytes_read = recv ( client, buffer, BUFFER_SIZE - 1, 0 );
        if ( bytes_read <= 0 )
        {
            perror ( "Chyba při čtení ze socketu: " );
            return false;
        }
        std::cout << "[READING] (size=" << bytes_read << "): " << buffer << std::endl;
        prev_buffer += std::string ( buffer, bytes_read );
        bzero ( buffer, BUFFER_SIZE );
        return true;
    }

    bool waitForMessage ( bool recharging )
    {
        if ( recharging )
            resetTimeoutRecharging();
        else
            resetTimeout();
        FD_ZERO ( &sockets );
        FD_SET ( client, &sockets );
        retval = select ( client + 1, &sockets, NULL, NULL, &timeout );
        if ( retval < 0 )
        {
            perror("Chyba při čekání na odpověď: ");
            return false;
        }
        if ( ! FD_ISSET ( client, &sockets ) )
        {
            std::cout << "TIMEOUT!" << std::endl;
            return false;
        }
        return true;
    }

    void resetTimeout()
    {
        timeout . tv_sec = TIMEOUT;
        timeout . tv_usec = 0;
    }

    void resetTimeoutRecharging()
    {
        timeout . tv_sec = TIMEOUT_RECHARGING;
        timeout . tv_usec = 0;
    }

    // Variables
    int client;

    timeval timeout;
    fd_set sockets;
    int retval;

    char buffer [ BUFFER_SIZE ];
    int bytes_read;
    std::string prev_buffer;
    std::string current_message;

    TCoordinates curr_pos;
    TCoordinates prev_pos;
    TCoordinates direction;
};

int main ( int argc, char **argv )
{
    int count = 0;
    if ( argc < 2 )
        return argCountError();

    int port = atoi ( argv [1] );
    int server;
    sockaddr_in address;
    if ( ! setUpServer ( port, server, address ) )
        return ERROR_VALUE;

    int client;
    sockaddr_in remote_address;
    socklen_t remote_address_len;
    while ( true )
    {
        count ++;
        if ( ! connectToClient ( server, client, remote_address, remote_address_len ) )
            return ERROR_VALUE;

        pid_t pid = fork();
        if ( notSubprocess ( pid, client ) )
            continue;

        std::cout << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << "   NEXT CLIENT [" << count <<  "]  " << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << std::endl;
        close ( server );
        CClientCommunication comm ( client );
        if ( ! comm . solve() )
            return ERROR_VALUE;
        return 0;
    }
    close ( server );
    return 0;
}