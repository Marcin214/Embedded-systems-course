typedef enum{SECCONDS, MINUTES}TimeUnit;

typedef struct{
	TimeUnit eTimeUnit;
	char TimeValue;
}WatchEvent;

void Watch_Init(void);
WatchEvent sWatch_Read(void);

