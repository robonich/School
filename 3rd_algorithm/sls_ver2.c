#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 1000000

int main(void){
    char data[LONGEST];
    int i = 0, j, k, l;
    FILE *fp;
    if((fp = fopen("short.txt", "r")) == NULL) {perror("fopen"); exit(1);}
    while((data[i] = fgetc(fp)) != EOF){
        if(data[i] == ' ' || data[i] == '\n') data[i] = '\0';
        else i++;
    }
    //printf("%s\n", data);
    
    int flag = 0;
    int syukai = 1;//何周目か
    
    while(flag == 0){
        for(j = 0; j < syukai; j++){
            for(i = 0; i < LONGEST - syukai; i++){
                if(data[i] != data[j+i]){
                    break;
                }
                else{
                    int j1 = j, i1 = i;
                    for(i = 0; i < i1; i++)
                    printf("%d")
                }
            }
        }
        syukai++;
    }
    fclose(fp);
    return 0;
}