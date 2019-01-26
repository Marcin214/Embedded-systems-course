enum KeyboardState {RELASED,BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

void KeyboardInit(void);
enum KeyboardState eReadButtons(void);
void Keyboard (void *pvParameters);

