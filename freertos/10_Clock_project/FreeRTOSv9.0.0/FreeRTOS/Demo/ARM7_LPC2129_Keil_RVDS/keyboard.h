enum KeyboardState {RELASED,BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

enum KeyboardState eReadButtons(void);

void Keyboard_Init(void);
enum KeyboardState eKeyboard_Read(void);
void Keyboard_Thread (void *pvParameters);
