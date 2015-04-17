/*----------------------------------------------------------------------------------------
- @Title: pincheck.c
- @Description: 
- @Author: Benjamin Bales
- @Email: bbales@mail.uoguelph.ca
- @Date:   2015-04-17 15:59:13
- @Last Modified time: 2015-04-17 17:34:24
-----------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pincheck.h"

int main() {
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

                tempPinList = pins[tempPin->num];
                if(tempPinList != NULL){
                    while(tempPinList->next != NULL){
                        tempPinList = tempPinList->next;
                    }
                    tempPinList->next = tempPin;
                }else{
                    pins[tempPin->num] = tempPin;
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
    listAll(pins);

    fclose(fp);
    return 0;
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
            printf("Pin %d (tqfp adapter: %d) = %s\n",i+1, equiv+1,tempPin->name);
            if(tempPin->desc != NULL) printf("     --> %s\n",tempPin->desc);
            tempPin = tempPin->next;
        }
    }

    return 1;
}