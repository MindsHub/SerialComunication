#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Serial.hpp"

int count=0;

int main(int argc, char * argv[]){
	//srand(time(NULL));
	//i2c_controller contr;
	if(argc!=2){
		printf("wrong parameter count\n");
	}
	//SerialDevice asseX("/dev/ttyACM0", 115200);
	SerialDevice test(argv[1], 115200);
	//test.send(0, 0, NULL);
	int correct=0;
	byte send[4];
	byte rec[4]; 
	for(int a=0; a<100000;a++){
		send[0]=rand();
		send[1]=rand();
		test.send('s', 2, send);
		usleep(200);
		test.receive(rec);
		if(rec[0]==(send[0]+send[1])%256){
			correct++;
		}else{
			printf("%d+%d=%d\n", send[0], send[1], rec[0]);
			exit(-1);
		}
		//
		printf("%d/%d\r", correct, a+1);
	}/**/
	
	return 0;
}
