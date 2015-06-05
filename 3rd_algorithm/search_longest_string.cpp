#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	char data[2000000];
	int i = 0;
	FILE fp = fopen("2015.txt", "r");
	if(fp == NULL) {perror("fopen"); exit(1);}
	while(fputc(data[i],fp) != EOF){
		if(data[i] == ' ' || data[i] == '\n');
		else i++;
	}
	printf("%s", data);
}