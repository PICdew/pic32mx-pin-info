typedef struct PIN {
    struct PIN * next;
    int num;
    char * desc;
    char * type;
    char * buffType;
    char * name;
} PIN;

int listAll(PIN ** pins);