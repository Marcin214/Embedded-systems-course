#define NULL 0
#define RECIEVER_SIZE 20
//**************************************************
typedef enum {EMPTY, READY, OVERFLOW}eRecieverStatus;

typedef struct{
char cData[RECIEVER_SIZE];
unsigned char ucCharCtr;
eRecieverStatus eStatus;
}RecieverBuffer;

//**************************************************
void UART_InitWithInt(unsigned int uiBaudRate);
//**************************************************
void Reciever_PutCharacterToBuffer(char cCharacter);
eRecieverStatus eReciever_GetStatus(void);
void Reciever_GetStringCopy(char * ucDestination);
//**************************************************