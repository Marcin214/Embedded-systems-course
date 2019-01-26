#define MAX_TOKEN_NR 						3        	//maksymalna dopuszczalna ilosc tokenów
#define MAX_KEYWORD_NR 					3         // maksymalna liczba komend
#define MAX_KEYWORD_STRING_LTH 20 	      // maksymalna dlugosc komendy


typedef enum { TOKEN, DELIMITER} TokenStatus;
typedef enum { KEYWORD, NUMBER, STRING} TokenType;
typedef enum { CALIB, GOTO, CALC} KeywordCode;

typedef struct{
	KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
}Keyword;

typedef union{
	KeywordCode eKeyword; 			// jezeli KEYWORD
	unsigned int uiNumber;      // jezeli NUMBER
	char * pcString;            // jezeli STRING
}TokenValue;
	
typedef struct{
	TokenType eType;    				// KEYWORD, NUMBER, STRING
	TokenValue uValue; 					// enum, unsigned int, char*
}Token;


/*----------------------------------------
		   Dekodowanie komunikatów
 *----------------------------------------*/
unsigned char ucFindTokensInString (char *pcString);
//Result eSringToKeyword(char pcStr[], KeywordCode *peKeywordCode);
void DecodeTokens(void);
void DecodeMsg(char *cStr);
