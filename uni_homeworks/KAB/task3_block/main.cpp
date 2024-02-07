#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h> // VYMAZAT

using namespace std;

struct crypto_config
{
	const char * m_crypto_function;
	std::unique_ptr<uint8_t[]> m_key;
	std::unique_ptr<uint8_t[]> m_IV;
	size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */

/*
Povinná hlavička 18 bytů - nešifrovat
Nepovinná část hlavičky vypočítaná z povinné hlavičky
Obrázková data - šifrovat

Pokud není validní klíč nebo IV, musí se vygenerovat nový a upravit se v configu. (klíč se mění podle typu šifry)

EVP_EncryptInit_ex, resp. EVP_DecryptInit_ex,
EVP_EncryptUpdate, resp. EVP_DecryptUpdate, - šifruje
EVP_EncryptFinal_ex, resp. EVP_EncryptFinal_ex. - padding -> větší výsledek než originál 1 blok šifry.

bloky PT zustavaji stejne
*/

const size_t READ_SIZE = 1024;
const size_t HEADER_SIZE = 18;

class CCrypto
{
  public:
    // Constructor, ...
    CCrypto ( const string& in_filename, const string& out_filename, crypto_config* configuration, bool dec )
    : config ( configuration ), deciphering ( dec )
    {
        inFile . open ( in_filename, ios_base::binary | ios_base::in );
        outFile . open ( out_filename, ios_base::binary | ios_base::out );
        OpenSSL_add_all_ciphers();
        ctx = EVP_CIPHER_CTX_new();
    }
    ~CCrypto ()
    {
        inFile . close();
        outFile . close();
        EVP_CIPHER_CTX_free ( ctx );
    }

    // Methods
    bool checkAndPrepare()
    {
        if ( ! inFile . is_open() || ! inFile . good()
            || ! outFile . is_open() || ! outFile . good() )
            return false;

        if ( config == nullptr )
            return false;

        if ( ctx == NULL )
            return false;

        if ( ! checkConfig() )
            return false;

        if ( ! copyHeader() )
            return false;

        return true;
    }

    bool encrypt()
    {
        if ( ! EVP_EncryptInit_ex ( ctx, cipher, NULL, config -> m_key.get(), config -> m_IV.get() ) )
            return false;

        int st_len = 0;
        while ( inFile . good() )
        {
            inFile . read ( reinterpret_cast<char*> ( ot_block ), READ_SIZE );
            if ( inFile . bad() )
                return false;

            if ( ! EVP_EncryptUpdate ( ctx, st_block, &st_len, ot_block, inFile . gcount() ) )
                return false;

            outFile . write ( reinterpret_cast<char*> ( st_block ), st_len );
            if ( ! outFile . good() )
                return false;
        }
        if ( ! EVP_EncryptFinal_ex ( ctx, st_block, &st_len ) )
            return false;

        outFile . write ( reinterpret_cast<char*> ( st_block ), st_len );
        if ( ! outFile . good() )
            return false;

        return true;
    }

    bool decrypt()
    {
        if ( ! EVP_DecryptInit_ex ( ctx, cipher, NULL, config -> m_key.get(), config -> m_IV.get() ) )
            return false;

        int ot_len = 0;
        while ( inFile . good() )
        {
            inFile . read ( reinterpret_cast<char*> ( st_block ), READ_SIZE );
            if ( inFile . bad() )
                return false;

            if ( ! EVP_DecryptUpdate ( ctx, ot_block, &ot_len, st_block, inFile . gcount() ) )
                return false;

            outFile . write ( reinterpret_cast<char*> ( ot_block ), ot_len );
            if ( ! outFile . good() )
                return false;
        }
        if ( ! EVP_DecryptFinal_ex ( ctx, ot_block, &ot_len ) )
            return false;

        outFile . write ( reinterpret_cast<char*> ( ot_block ), ot_len );
        if ( ! outFile . good() )
            return false;

        return true;
    }

    // Variables

  private:
    // Methods
    bool copyHeader()
    {
        inFile . read ( reinterpret_cast<char*> ( header ), HEADER_SIZE );
        if ( inFile . bad()
            || inFile . gcount() != HEADER_SIZE )
            return false;

        outFile . write ( reinterpret_cast<char*> ( header ), HEADER_SIZE );
        if ( ! outFile . good() )
            return false;

        return true;
    }

    bool checkConfig()
    {
        cipher = EVP_get_cipherbyname ( config -> m_crypto_function );
        if ( ! cipher )
            return false;

        size_t key_len = EVP_CIPHER_key_length ( cipher );
        size_t iv_len = EVP_CIPHER_iv_length ( cipher );

        if ( ! validKey ( key_len ) )
        {
            if ( deciphering || ! getKey ( key_len ) )
                return false;
        }

        if ( ! validIV ( iv_len ) )
        {
            if ( deciphering || ! getIV ( iv_len ) )
                return false;
        }
        return true;
    }

    bool validKey ( size_t len )
    {
        if ( config -> m_key_len < len || config -> m_key == nullptr )
            return false;

        return true;
    }

    bool validIV ( size_t len )
    {
        if ( len == 0 )
            return true;
        if ( config -> m_IV_len < len || config -> m_IV == nullptr )
            return false;
        return true;
    }

    bool getKey ( size_t len )
    {
        config -> m_key_len = len;

        config -> m_key = make_unique<uint8_t[]> ( len );
        if ( config -> m_key == nullptr )
            return false;

        RAND_bytes ( reinterpret_cast<unsigned char*> ( config -> m_key . get() ), len );
        return true;
    }

    bool getIV ( size_t len )
    {
        config -> m_IV_len = len;
        
        config -> m_IV = make_unique<uint8_t[]> ( len );
        if ( config -> m_IV == nullptr )
            return false;

        RAND_bytes ( reinterpret_cast<unsigned char*> ( config -> m_IV . get() ), len );
        return true;
    }

    // Variables

    ifstream inFile;
    ofstream outFile;
    crypto_config* config;

    unsigned char header [ HEADER_SIZE ]; // header
    unsigned char ot_block [ READ_SIZE + 2 * EVP_MAX_BLOCK_LENGTH ]; // open text
    unsigned char st_block [ READ_SIZE + 2 * EVP_MAX_BLOCK_LENGTH ]; // sifrovany text

    EVP_CIPHER_CTX* ctx;
    const EVP_CIPHER* cipher;

    bool deciphering;
};

/**
 * @param in_filename Input file.
 * @param out_filename Input file.
 * @param config Input file.
 * @return True if OK, false if mistake (invalid file - missing head / open/read/write error, crypto_config unsuccesfull change).
*/
bool encrypt_data ( const std::string& in_filename, const std::string& out_filename, crypto_config& config )
{
    CCrypto crypto ( in_filename, out_filename, &config, false );
    if ( ! crypto . checkAndPrepare() )
        return false;

    if ( ! crypto . encrypt() )
        return false;

    return true;
}

/**
 * @param in_filename Input file.
 * @param out_filename Input file.
 * @param config Input file.
 * @return True if OK, false if mistake (invalid key or IV).
*/
bool decrypt_data ( const std::string& in_filename, const std::string& out_filename, crypto_config& config )
{
    CCrypto crypto ( in_filename, out_filename, &config, true );

    if ( ! crypto . checkAndPrepare() )
        return false;

    if ( ! crypto . decrypt() )
        return false;

    return true;
}


#ifndef __PROGTEST__

bool compare_files ( const char* name1, const char* name2 )
{
    size_t counter = 0;
    ifstream f1, f2;
    f1 . open ( name1, ios_base::binary ); // first file
    f2 . open ( name2, ios_base::binary ); // second file

    if ( ! f1 . is_open() || ! f2 . is_open() ) // if error while opening files occured
        return false;

    char c1, c2;
    while ( f1 . get ( c1 ) && f2 . get ( c2 ) ) // while characters in both streams
    {
        counter ++;
        if ( c1 != c2 ) // character mismatch
            return false;
    }
    f2 . get ( c2 );
    if ( f1 . eof() != f2 . eof() )
        return false;
    f1 . close ();
    f2 . close ();
    return true;
}

int main ( void )
{
	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
	config.m_crypto_function = "AES-128-ECB";

    // nullptr, správná délka - IV, key
	config.m_key = nullptr;
	config.m_key_len = 16;

	config.m_IV = std::make_unique<uint8_t[]>(16);
	memset(config.m_IV.get(), 0, 16);
	config.m_IV_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );

	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) &&
			compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

    // správný klíč, špatná délka - IV,key
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 4;

	config.m_IV = nullptr;
	config.m_IV_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );

	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) &&
			compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

    // nullptr, špatná délka
	config.m_key = nullptr;
	config.m_key_len = 4;

	config.m_IV = std::make_unique<uint8_t[]>(16);
	memset(config.m_IV.get(), 0, 16);
	config.m_IV_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );

	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) &&
			compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";

    // nullptr, správná délka - IV, key
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 0;

	config.m_IV = nullptr;
	config.m_IV_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );
	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) );
    assert ( compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

    // správný klíč, špatná délka - IV,key
	config.m_key = nullptr;
	config.m_key_len = 4;

	config.m_IV = std::make_unique<uint8_t[]>(16);
	memset(config.m_IV.get(), 0, 16);
	config.m_IV_len = 0;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );

	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) );
    assert ( compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

    // nullptr, špatná délka
	config.m_key = nullptr;
	config.m_key_len = 16;

	config.m_IV = nullptr;
	config.m_IV_len = 0;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) );

	assert( decrypt_data  ("out_file.TGA", "out_file_test.TGA", config) );
    assert ( compare_files ("out_file_test.TGA", "homer-simpson.TGA") );

    // PROGTEST
	// ECB mode
	config.m_crypto_function = "AES-128-ECB";
	config.m_key = std::make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

	assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

	assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

	assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";
	config.m_IV = std::make_unique<uint8_t[]>(16);
	config.m_IV_len = 16;
	memset(config.m_IV.get(), 0, 16);

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

	assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

	assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

	assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );
	return 0;
}

#endif /* _PROGTEST_ */
