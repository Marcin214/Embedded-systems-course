enum DetectorState{ACTIVE,INACTIVE};

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo{
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};



void InitTimer0(void);
void WaitOnTimer0(unsigned int uiTime);
void InitTimer0Match0(unsigned int iDelayTime);
void WaitOnTimer0Match0(void);
