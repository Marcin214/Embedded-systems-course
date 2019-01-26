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
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);


void Automat2(void);
