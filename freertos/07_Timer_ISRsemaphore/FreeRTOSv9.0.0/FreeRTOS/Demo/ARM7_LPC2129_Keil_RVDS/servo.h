enum ServoState {CALLIB, IDLE, IN_PROGRESS};
enum DetectorState{ACTIVE,INACTIVE};

struct Servo{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

void Automat(void);
void DetectorInit(void); // ustawienie detektora jako wejscie
enum DetectorState eReadDetector(void); // zczytanie stanu DETEKTORA
void ServoInit(unsigned int uiServoFrequency);


//***** DATA STRUCTURE SENDING TO QUEUE *****

/* Define an enumerated type used to identify the source of the data. */
typedef enum{_GOTO, _CALLIB, _SPEED, _WAIT }DataSource;

/* Define an union type used to passed value of the data. */
typedef union{
unsigned char ucValue;
unsigned int  uiValue;
} DataValue;

 /* Define the structure type that will be passed on the queue. */
typedef struct{
	DataSource eDataSource;
	DataValue  uDataValue;
}Data;

//*************** SERVO API *****************
void Automats(void *pvParameters);
void Servo_Callib(void);
void Servo_GoTo(unsigned char ucPosition);
void Servo_Wait(unsigned int uiTimeToWait);
void Servo_Speed(unsigned char ucServoSpeed);



