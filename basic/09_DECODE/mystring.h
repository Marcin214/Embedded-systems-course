#define MAX_TOKEN_NR 3            	//maksymalna dopuszczalna ilosc tokenów
#define MAX_KEYWORD_NR 2            // maksymalna liczba komend
#define MAX_KEYWORD_STRING_LTH 20 	// maksymalna dlugosc komendy
#define NIBBLE 0x0000000F
#define SPACE 0x20
#define TERMINATOR 0xD  


typedef enum { TOKEN, DELIMITER} TokenStatus;
typedef enum { KEYWORD, NUMBER, STRING} TokenType;
typedef enum { CALIB, GOTO} KeywordCode;
typedef enum { OK, ERRORR} Result;
typedef enum { DIFFERENT , EQUAL } CompResult;


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
		 Lancuchy znakowe - operacje proste
 *----------------------------------------*/
void CopyString(char pcSource[], char pcDestination[]);
CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);

/*----------------------------------------
		 Lancuchy znakowe - konwersje
 *----------------------------------------*/
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);

/*----------------------------------------
		   Dekodowanie komunikatów
 *----------------------------------------*/
unsigned char ucFindTokensInString (char *pcString);
Result eSringToKeyword(char pcStr[], KeywordCode *peKeywordCode);
void DecodeTokens(void);
void DecodeMsg(char *cStr);
