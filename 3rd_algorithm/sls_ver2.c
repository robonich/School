#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define LONGEST 615//1000000

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
            char *longest_string;
            longest_string = (char *)malloc(sizeof(char)*LONGEST);
            for(i = 0; i < LONGEST - syukai; i++){
                *(longest_string+i) = data[j+i];
            }
            *(longest_string+i) = '\0';
            //printf("%s\n", longest_string);
            for(k = j+1; k < syukai+1; k++){
                l = 0;
                while(*(longest_string+l) != '\0'){
                    if(*(longest_string+l) != data[k+l]) break;
                    else l++;
                }
                if(*(longest_string+l) == '\0'){
                    flag = 1;
                    break;
                } 
            }
            if(flag == 1){printf("the longest string is %s\n", longest_string); break;}
            free(longest_string);
        }
        syukai++;
    }
    fclose(fp);
    return 0;
}