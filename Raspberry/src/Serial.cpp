#include<Serial.hpp>


const char *loggerFile = (const char *) "./log.txt";

int addr = 0x10;
int file_i2c = -1;

char * trim(char * message) {
	message[strlen(message)-1] = 0;
	return message;
}

void i2c_saveError(char *message) {
	FILE *output = fopen(loggerFile, "a");
	if(output==NULL){
		printf("FANCULO |%s|\n", loggerFile);
		fflush(stdout);
	}else{
		time_t currentTime = time(NULL);
		fprintf(output,"%s %s\n", trim(ctime(&currentTime)), message);
		fclose(output);
	}
	
	

	printf("\n%s\n", message);

}

