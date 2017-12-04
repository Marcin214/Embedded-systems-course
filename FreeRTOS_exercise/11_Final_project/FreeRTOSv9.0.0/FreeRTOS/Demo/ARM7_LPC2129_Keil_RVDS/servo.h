enum ServoState {CALLIB, IDLE, IN_PROGRESS, WAIT};
enum DetectorState{ACTIVE,INACTIVE};

struct ServoStatus{
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


 /* Define the structure type that will be passed on the queue. */
typedef struct{
	DataSource eDataSource;
	unsigned int  uiValue;
}Data;

//*************** SERVO API *****************
void Automats(void *pvParameters);
void Servo_Callib(void);
void Servo_GoTo(unsigned int uiPosition);
void Servo_Wait(unsigned int uiTimeToWait);
void Servo_Speed(unsigned int uiServoSpeed);
struct ServoStatus Servo_State(void);



