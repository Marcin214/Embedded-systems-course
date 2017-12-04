#include <stdio.h>     
#include "string.h"    
#define TERMINATOR 0xD  

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
