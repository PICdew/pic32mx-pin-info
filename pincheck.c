/*----------------------------------------------------------------------------------------
- @Title: pincheck.c
- @Description: 
- @Author: Benjamin Bales
- @Email: bbales@mail.uoguelph.ca
- @Date:   2015-04-17 15:59:13
- @Last Modified time: 2015-04-18 13:58:59
-----------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pincheck.h"

int main(int argc, char *argv[]) {
    FILE * fp;
    fp = fopen("PIC32MX5xxPINS.txt","r");
    PIN ** pins = malloc(sizeof(PIN*)*64);
    char * tempStr;
    int tempLen = 0;
    char * nameStr;
    int nameLen = 0;
    char c;
    int stage = 0;
    PIN * tempPin;
    PIN * tempPinList;

    while((c = fgetc(fp)) != EOF){
        if(c == '\n'){
            if(stage != 0){
                if(tempLen != 0 && stage == 7){
                    tempPin->desc = tempStr;
                }else{
                    tempPin->desc = NULL;
                }

                tempPinList = pins[tempPin->num-1];
                if(tempPinList != NULL){
                    while(tempPinList->next != NULL){
                        tempPinList = tempPinList->next;
                    }
                    tempPinList->next = tempPin;
                }else{
                    pins[tempPin->num-1] = tempPin;
                }
            }

            stage = 0;
            continue;
        }
        switch(stage){
            case 0:
                if(c == ' '){
                    nameLen = 0;
                    tempLen  = 0;
                    stage++;
                }else{
                    if(!nameLen){
                        nameStr = malloc(sizeof(char)*2);
                        nameLen++;
                    }else{
                        nameLen++;
                        nameStr = realloc(nameStr,sizeof(char)*(nameLen+1));
                    }

                    nameStr[nameLen-1] = c;
                    nameStr[nameLen] = '\0';
                }
                break;
            case 1:
                if(c == ' '){
                    tempPin = malloc(sizeof(PIN));
                    tempPin->num = atoi(tempStr);
                    tempPin->name = nameStr;
                    tempPin->next = NULL;

                    tempLen = 0;
                    stage++;
                }else{
                    if(!tempLen){
                        tempStr = malloc(sizeof(char)*2);
                        tempLen++;
                    }else{
                        tempLen++;
                        tempStr = realloc(tempStr,sizeof(char)*(tempLen+1));
                    }

                    tempStr[tempLen-1] = c;
                    tempStr[tempLen] = '\0';
                }
                break;

            case 2:
            case 3:
            case 4:
                if(c == ' '){
                    stage++;
                }else{
                    continue;
                }
                break;
            case 5:
                if(c == ' '){
                    tempPin->type = tempStr;
                    tempLen = 0;
                    stage++;
                }else{
                    if(!tempLen){
                        tempStr = malloc(sizeof(char)*2);
                        tempLen++;
                    }else{
                        tempLen++;
                        tempStr = realloc(tempStr,sizeof(char)*(tempLen+1));
                    }

                    tempStr[tempLen-1] = c;
                    tempStr[tempLen] = '\0';
                }
                break;
            case 6:
                if(c == ' '){
                    tempPin->buffType = tempStr;
                    tempLen = 0;
                    stage++;
                }else{
                    if(!tempLen){
                        tempStr = malloc(sizeof(char)*2);
                        tempLen++;
                    }else{
                        tempLen++;
                        tempStr = realloc(tempStr,sizeof(char)*(tempLen+1));
                    }

                    tempStr[tempLen-1] = c;
                    tempStr[tempLen] = '\0';
                }
                break;
            case 7:
                if(!tempLen){
                    tempStr = malloc(sizeof(char)*2);
                    tempLen++;
                }else{
                    tempLen++;
                    tempStr = realloc(tempStr,sizeof(char)*(tempLen+1));
                }

                tempStr[tempLen-1] = c;
                tempStr[tempLen] = '\0';
                break;
        }
    }
    
    if(argc == 1){
        listOptions();
    }else if(argc == 2){
        if(strcmp("-a",argv[1]) == 0){
            listAll(pins);
        }
    }else if(argc == 3){
        if(strcmp("-p",argv[1]) == 0){
            pinInfo(atoi(argv[2]),pins);
        }else
        if(strcmp("-r",argv[1]) == 0){
            revPinInfo(atoi(argv[2]),pins);
        }else
        if(strcmp("-s",argv[1]) == 0 || strcmp("-f",argv[1]) == 0){
            searchName(pins,argv[2]);
        }else{
            printf("Option does not exist.\n");
        }
    }

    fclose(fp);
    return 0;
}

int listOptions(){
    printf("Available options: \n\n    -a : list all pins and their information.\n");
    printf("    -p [num] : show information for physical pin.\n");
    printf("    -r [num] : show information for tqfp adapter pin.\n");
    printf("    -f [keyword] : Search for pins based on name.\n");
    printf("\n");
    return 1;
}

int pinInfo(int num, PIN ** pins){
    int i,equiv;
    PIN * tempPin;

    i = num-1;

    if(i < 16){
        equiv = i;
    }else if(i < 32){
        equiv = i + 9;
    }else if(i < 48){
        equiv = i + 27;
    }else if(i < 64){
        equiv = i + 36;
    }
    
    tempPin = pins[i];

    printf("\nPin %d Summary\n",i+1);
    printf("\n    TQFP 100 adapter equivalent pin: %d\n",equiv+1);
    while(tempPin != NULL){
        printf("    --> %s",tempPin->name);
        if(tempPin->desc != NULL) printf(" : %s",tempPin->desc);
        printf("\n");
        tempPin = tempPin->next;
    }

    printf("\n");

    cleanUp(pins);

    return 1;
}

int cleanUp(PIN ** pins){
    int i;
    PIN* tempPin;
    PIN* tempPin2;
    for(i=0;i<64;i++){
        tempPin = pins[i];
        while(tempPin != NULL){
            free(tempPin->name);
            free(tempPin->desc);
            free(tempPin->type);
            free(tempPin->buffType);
            tempPin2 = tempPin->next;
            free(tempPin);
            tempPin = tempPin2;
        }
    }
    return 1;
}

int revPinInfo(int num, PIN ** pins){
    int i,equiv;
    PIN * tempPin;

    i = num-1;

    if(i < 25){
        equiv = i;
    }else if(i < 50){
        equiv = i - 9;
    }else if(i < 75){
        equiv = i - 27;
    }else if(i < 100){
        equiv = i - 36;
    }
    
    tempPin = pins[equiv];

    printf("\nPin %d Summary\n",i+1);
    printf("    TQFP 100 adapter equivalent pin: %d\n",equiv+1);
    while(tempPin != NULL){
        printf("    --> %s",tempPin->name);
        if(tempPin->desc != NULL) printf(" : %s",tempPin->desc);
        printf("\n");
        tempPin = tempPin->next;
    }

    printf("\n");

    return 1;
}

int listAll(PIN ** pins){
    int i,equiv;
    PIN * tempPin;

    for(i = 0; i < 64; i++){
        if(i < 16){
            equiv = i;
        }else if(i < 32){
            equiv = i + 9;
        }else if(i < 48){
            equiv = i + 27;
        }else if(i < 64){
            equiv = i + 36;
        }
        tempPin = pins[i];
        while(tempPin != NULL){
            printf("PIN %d (tqfp adapter: %d) = %s\n",i+1, equiv+1,tempPin->name);
            if(tempPin->desc != NULL) printf("     --> %s\n",tempPin->desc);
            tempPin = tempPin->next;
        }
    }

    return 1;
}

int searchName(PIN ** pins, char * keyword){
    int i;
    int matches = 0;
    PIN * tempPin;
    printf("\nSearch Results:\n\n");
    for(i=0;i<64;i++){
        tempPin = pins[i];
        while(tempPin != NULL){
            if(!strncmp(keyword,tempPin->name,strlen(keyword))){
                matches++;
                printf("    %s  <---> %d\n",tempPin->name,tempPin->num);
            }
            tempPin = tempPin->next;
        }
    }
    if(!matches){
        printf("    No matches found.\n\n");
    }else{
        printf("\n%d matches found.\n",matches);
    }
    return 1;
}