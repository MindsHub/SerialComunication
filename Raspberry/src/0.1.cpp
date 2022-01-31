#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Serial.hpp"

int count=0;

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
}

int main(int argc, char * argv[]){
	//srand(time(NULL));
	//i2c_controller contr;
	if(argc!=2){
		printf("wrong parameter count\n");
	}
	//SerialDevice asseX("/dev/ttyACM0", 115200);
	SerialController test(argv[1], 115200);
	int file=test.serialFile;
	char out[50], inp[50];
	memset(out, 0, 50);
	memset(inp, 0, 50);
	printf("starting\n");
	time_t start = clock();
	for(int a=0; a<100000; a++){
		sprintf(out, "%10d", a);
		//printf("|%s|\n",out);
		int sent=write(file, out, strlen(out));
		//tcflush(file,TCOFLUSH);
		//
		//
		//usleep(50);
		int val=read(file, inp, strlen(out));
		
		
		
		if(strcmp(inp, out)!=0||val!=strlen(out)){
			printf("error on %d\n|%s|\n|%s|\n", a, inp, out);
			printf("sent%d received %d\n",sent, val, (strlen(out)+1));
			usleep(5000);
			tcflush(file,TCIFLUSH);
		}/**/
	}
	printf("%f", ((float)clock()-start)/CLOCKS_PER_SEC);
	return 0;
}
