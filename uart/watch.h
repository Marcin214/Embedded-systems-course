typedef struct{
	unsigned char ucMinutes, ucSecconds;
	unsigned char fSeccondsValueChanged, fMinutesValueChanged;
} Watch;

void WatchUpdate(void);
void WatchInit(void);

