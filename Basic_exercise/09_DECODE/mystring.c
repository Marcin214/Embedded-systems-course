#include <stdio.h>     
#include "mystring.h"    

Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;
Keyword asKeywordList[MAX_KEYWORD_NR]= 
{
	{CALIB,"callib"},	
	{GOTO, "goto" }
};

/*----------------------------------------
		 Lancuchy znakowe - operacje proste
 *----------------------------------------*/
void CopyString(char pcSource[], char pcDestination[]){
	unsigned int uiLength;
	for(uiLength=0;  pcSource[uiLength]!= NULL; uiLength++){
		 pcDestination[uiLength]=pcSource[uiLength];
		}
	 pcDestination[uiLength++]=NULL;
}
//_____________________________________________________________
CompResult eCompareString(char pcStr1[], char pcStr2[]){
	for(unsigned int uiLength=0;((pcStr1[uiLength]!= NULL) || (pcStr2[uiLength] != NULL)); ++uiLength){
		if(pcStr1[uiLength] != pcStr2[uiLength])
			return DIFFERENT;		
	}
	return EQUAL;
}
//_____________________________________________________________
void AppendString (char pcSourceStr[],char pcDestinationStr[]){
	
	unsigned int uiLength1,uiLength2;
	for(uiLength1=0;pcSourceStr[uiLength1] != NULL; uiLength1++){}
	for(uiLength2=0;pcDestinationStr[uiLength2] != NULL; uiLength2++){}
	CopyString(pcSourceStr, pcDestinationStr+uiLength2);
		pcDestinationStr[uiLength1+uiLength2] = NULL;
}
//_____________________________________________________________
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar){
	for(unsigned int uiLength=0;pcString[uiLength] != NULL; uiLength++){
		if(pcString[uiLength] == cOldChar)
			pcString[uiLength] = cNewChar;
	}
}
/*----------------------------------------
		 Lancuchy znakowe - konwersje
 *----------------------------------------*/

void UIntToHexStr (unsigned int uiValue, char pcStr[]){
  unsigned int uiDecimal=uiValue;
	for(unsigned int uiLength=0;uiLength < 7; uiLength++){
	if(uiLength == 0)
		*(pcStr+uiLength) = '0';
	else if(uiLength == 1)
		*(pcStr+uiLength) = 'x';
	else if(uiLength == 6)
		*(pcStr+uiLength) = NULL;
	else{	
			if((uiDecimal&0x0F)<10)
				*(pcStr+(7-uiLength)) = (uiDecimal&NIBBLE)+48;
			else
			  *(pcStr+(7-uiLength)) = (uiDecimal&NIBBLE)+55;
			uiDecimal>>=4;	
		}
	}
}
//_____________________________________________________________
Result eHexStringToUInt(char pcStr[],unsigned int *puiValue){
	
	if((pcStr[0] != '0')|| (pcStr[1] != 'x') || (pcStr[6] != '\0')){
		return ERRORR;
	}
	for(unsigned int uiLength=5, uiRate=1;uiLength > 1; uiLength--,uiRate<<=4){
	
		if(pcStr[uiLength] < 58){
		*puiValue += ((pcStr[uiLength]-48)*uiRate);	
		}
		else{
		*puiValue += ((pcStr[uiLength]-55)*uiRate);	
		}
	}
	return OK;
}
//_____________________________________________________________
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){
	
	unsigned int uiLengthStr;
	for(uiLengthStr=0;pcDestinationStr[uiLengthStr] != NULL; uiLengthStr++);

	for(unsigned int uiLength=0;uiLength < 7; uiLength++){
	if(uiLength == 0)
		*(pcDestinationStr+uiLength+uiLengthStr) = '0';
	else if(uiLength == 1)
		*(pcDestinationStr+uiLength+uiLengthStr) = 'x';
	else if(uiLength == 6)
		*(pcDestinationStr+uiLength+uiLengthStr) = NULL;
	else{	
			if((uiValue&0x0F)<10)
				*(pcDestinationStr+(7-uiLength+uiLengthStr)) = (uiValue&NIBBLE)+48;
			else
			  *(pcDestinationStr+(7-uiLength+uiLengthStr)) = (uiValue&NIBBLE)+55;
			uiValue>>=4;	
			}
		}
	}
/*----------------------------------------
		   Dekodowanie komunikatów
 *----------------------------------------*/
	unsigned char ucFindTokensInString (char *pcString) {

  TokenStatus eState = DELIMITER;
	ucTokenNr = 0;

    for (unsigned int uiLength=0;; uiLength++) {
			switch (eState) {
	    	case TOKEN:
					if((pcString[uiLength] == NULL)||(ucTokenNr == MAX_TOKEN_NR))
		    			return ucTokenNr;
					else if(pcString[uiLength] == SPACE)
		    			eState=DELIMITER;
					else
						eState = TOKEN;
					break;
	    		
				case DELIMITER:
					if (pcString[uiLength] == NULL)
		    			return ucTokenNr;
					else if (pcString[uiLength] == SPACE) 
						eState = DELIMITER;
					else{
		    			eState = TOKEN;
		    			asToken[ucTokenNr].uValue.pcString = pcString + uiLength;
							ucTokenNr++;
					}
					break;
			}
    }
    return 0;
}

//_____________________________________________________________
Result eStringToKeyword (char cStr[],KeywordCode *peKeyword) {

	unsigned char ucKeywordCtr;

	for (ucKeywordCtr = 0; ucKeywordCtr < MAX_KEYWORD_NR; ucKeywordCtr ++) {
		if (eCompareString (cStr, asKeywordList[ucKeywordCtr].cString) == EQUAL) {
			*peKeyword = asKeywordList[ucKeywordCtr].eCode;
			return OK;
		}
	}
	return ERRORR;
}
//_____________________________________________________________
void DecodeTokens(void) {

	unsigned char ucTokenCtr;

	for (ucTokenCtr=0; ucTokenCtr < ucTokenNr ; ucTokenCtr ++ ) {
		KeywordCode eKeywordCode;
		unsigned int uiNumberValue;
		Token * pasToken;
		pasToken = &asToken[ucTokenCtr];

		if (eStringToKeyword(pasToken ->uValue.pcString, &eKeywordCode) == OK) {
			pasToken->eType = KEYWORD;
			pasToken->uValue.eKeyword = eKeywordCode;
		}
		else if (eHexStringToUInt(pasToken ->uValue.pcString, &uiNumberValue) == OK) {
			pasToken->eType = NUMBER;
			pasToken->uValue.uiNumber = uiNumberValue;
		}
		else {
			pasToken ->eType = STRING;
		}
	}
}
//_____________________________________________________________
void DecodeMsg(char *cStr) {
ucFindTokensInString(cStr);
	ReplaceCharactersInString(cStr,SPACE,NULL);
	DecodeTokens();
}

