typedef struct {
    char *str;
    unsigned int pos;
    unsigned int length;
} TokenReader;

void setTokenString(TokenReader *reader, char* str);
TokenReader* newTokenReader(char *str);
void freeTokenReader(TokenReader *reader);
int hasMoreTokens(TokenReader *reader);
char* nextToken(TokenReader *reader);
