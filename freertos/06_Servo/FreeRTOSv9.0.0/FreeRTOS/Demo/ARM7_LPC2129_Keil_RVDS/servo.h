

void Automat(void);
void DetectorInit(void); // ustawienie detektora jako wejscie
enum DetectorState eReadDetector(void); // zczytanie stanu DETEKTORA
void ServoInit(unsigned int uiServoFrequency);


//*************** SERVO API *****************
void Automats(void *pvParameters);
void Servo_Callib(void);
void Servo_GoTo(unsigned char ucPosition);
void Servo_Wait(unsigned int uiTimeToWait);
void Servo_Speed(unsigned char ucServoSpeed);



