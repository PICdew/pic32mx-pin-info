typedef struct PIN {
    struct PIN * next;
    int num;
    char * desc;
    char * type;
    char * buffType;
    char * name;
} PIN;

int listOptions();
int pinInfo(int num, PIN ** pins);
int revPinInfo(int num, PIN ** pins);
int listAll(PIN ** pins);