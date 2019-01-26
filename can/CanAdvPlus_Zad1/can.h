void Can1_InitAsTX(char acTxBuffer[]);
void Can2_InitAsRX(char acRxBuffer[]);
unsigned char  ucCan1_TxFree(void);
unsigned char  ucCan2_RxReady(void);
void Can1_SendByte(unsigned char ucValue);
unsigned char ucCan2_ReceiveByte(void);
void Can1_SendString(void); 

