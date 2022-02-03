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
	SerialController test(argv[1], 115200);
	int file=test.serialFile;
	char out[50], inp[50];
	memset(out, 0, 50);
	memset(inp, 0, 50);
	printf("starting\n");
	time_t start = clock();
	for(int a=0; a<100000; a++){
		sprintf(out, "%d", a%10);
		//printf("|%s|\n",out);
		int sent=write(file, out, strlen(out));
		//tcflush(file,TCOFLUSH);
		//
		//
		usleep(50);
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
