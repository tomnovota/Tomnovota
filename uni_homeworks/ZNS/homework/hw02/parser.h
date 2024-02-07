#ifndef PARSER_H
#define PARSER_H

#define MAX_IDENT_LEN   32

typedef enum { P_NULL, P_LEFT, P_RIGHT, P_COMMA, P_IDENT, P_EQUAL, P_NOTEQUAL } TParserProductType;

typedef struct {
    TParserProductType type;
    char value[MAX_IDENT_LEN];
} TParserData;

TParserData parser( const char **source);
void parserCheck( TParserProductType exceptedType, TParserData &data, const char *line);

#endif // PARSER_H
