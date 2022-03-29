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
	byte send[] = "send";
	byte rec[4]; 
	for(int a=0; a<2;a++){
		test.send(a%256, 4, send);
		usleep(200);
		if(a%256==test.receive(rec)){
			correct++;
		}
		printf("%d/%d\n", correct, a+1);
	}/**/
	
	return 0;
}
