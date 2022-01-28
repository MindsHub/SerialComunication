#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Serial.hpp"

int count=0;
/*
void move(SerialDevice dev, int32_t pos){
	printf("\rM->%5d %d\n", pos, ++count);
	fflush(stdout);
	unsigned char buf[5];
	buf[0]='M';
	memcpy(buf+1,&pos, 4);
	dev.delivery(buf, 5);
}

void sum(SerialDevice dev, int32_t x, int32_t y){
	printf("%d+%d=",x,y);
	unsigned char buf[9];
	unsigned char* out;
	buf[0]='S';
	memcpy(buf+1,&x, 4);
	memcpy(buf+5,&y, 4);
	int32_t somma=x+y;
	unsigned char size=dev.delivery(buf, 9, &out);
	if(size==4){
		memcpy(&x,out, 4);
		printf("%d %d\n",x, ++count);
		if(somma!=x){
			printf("erroraccio\n");
			exit(-1);
		}
	}
}*/

int main(){
	srand(time(NULL));
	//i2c_controller contr;
	
	//SerialDevice asseX("/dev/ttyACM0");
	SerialController asseX("/dev/ttyACM0", 460800);
	int file=asseX.serialFile;
	char buf[20];
	char inp[20];
	//usleep(2000000);
	int correct=0;
	for(int a=0; a<100000; a++){
		sprintf(buf, "%d", a);
		int written=write(file,buf, strlen(buf)+1);
		usleep(6*9*2*(strlen(buf)+1));
		int readen=read(file, inp, strlen(buf)+1);
		usleep(6*9*2*(strlen(buf)+1));
		
		/*for(int a=0;a<5;a++){
			printf("%d ", buf[a]);
		}
		printf("\n");*/
		if(strcmp(buf, inp)==0){
			correct++;
		}else{
			printf("%d |%s|\n", a, inp);
			printf("written %d\n", written);
			printf("readen %d\n", readen);
			exit(-1);
		}
		
	}
	printf("%d/%d\n", correct, 100000);
	
	
	//unsigned char test[3];
	//asseX.writeComand(0, 2);
	//asseX.readComand(0);
	//setup();
	/*for(int a=0;a<10000;a++){
		move((int32_t)rand()%(200*32));
		usleep(1000000);
	}*/
	//move(asseX, 1000);
	/*for(int a=0;a<5000;a++){
			
		//sum(asseX, rand()%100, rand()%100);
		move(asseX, rand()%(200*32));
		//usleep(10000);
	}*/
	//i2c_write_comand(0, 'M');
}
