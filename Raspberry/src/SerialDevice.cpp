#include<Serial.hpp>

SerialDevice::SerialDevice(char * filename, int baudRate){
	serial.setup(filename, baudRate);
	serialFile=serial.serialFile;
}

void SerialDevice::reload(){
	printf("reload\n");
	serial.reload();
	serialFile=serial.serialFile;
}

bool SerialDevice::validHeader(byte* data){
	if(data[0]!=17){
		printf("invalid header start\n");
		return false;
	}
	byte indexChecksum=data[1]+data[2]+data[3]+data[4];
	if(indexChecksum!=data[5]){
		printf("invalid header checksum\n");
		return false;
	}
	return true;
}

bool SerialDevice::validData(byte* data, byte dataSize, byte dataChecksum, byte progressiveChecksum){
	/*if(data[0]!=17){
		printf("invalid data start\n");
		return false;
	}*/
	byte checksum=0;
	byte pChecksum=0;
	for(int a=0; a<dataSize; a++){
		checksum+=data[a];
		pChecksum+=(a+1)*data[a];
	}
	if(checksum!=dataChecksum||pChecksum!=progressiveChecksum){
		printf("invalid data checksum\n");
		return false;
	}
	return true;
}

void SerialDevice::send(byte cmd, byte dataSize, byte* data){//XON-int cmd-int dataSize-int data checksum - int progressive- int index checksum - XOFF
	byte msg[6];
	byte dataChecksum=0;
	byte progressiveChecksum=0;
	if(dataSize>0){
		for(int a=0; a<dataSize; a++){
			dataChecksum+=data[a];
			progressiveChecksum+=(a+1)*data[a];
		}
	}
	

	msg[0] = 17;//XON
	msg[1] = cmd;
	msg[2] = dataSize;
	msg[3] = dataChecksum;
	msg[4] = progressiveChecksum;
	byte indexChecksum = cmd+dataSize+dataChecksum+progressiveChecksum;
	msg[5] = indexChecksum;
	printf("writing: %d\n", cmd);
	write(serialFile, msg, 6);
	
	usleep(SERIAL_DELAY*6);
	if(dataSize>=1){
		write(serialFile, data, dataSize);
		usleep(SERIAL_DELAY*dataSize);
	}
	#ifdef DEBUG_BINARY
		for(int a=0; a<6; a++){
			for(int b=7; b>=0; b--){
				printf("%d",(msg[a]>>b)%2);
			}
			printf(" ");
		}
		printf(" || ");
		for(int a=0; a<dataSize; a++){
			for(int b=7; b>=0; b--){
				printf("%d",(data[a]>>b)%2);
			}
			printf(" ");
		}
		printf("\n");
	#endif
	//write(serialFile, msg+6, 1);//file end
	//usleep(SERIAL_DELAY);
	//printf("end: %d\n", cmd);
}

int SerialDevice::receive(byte* data){//XON-int cmd-int dataSize-int data checksum - int progressive- int index checksum - XOFF
	byte msg[8];
	byte buffer[258];
	int nread;
	usleep(SERIAL_DELAY*6);
	if((nread=read(serialFile, msg, 6))!=6){
		printf("error reading header: %d/8\n", nread);
		exit(-1);
	}
	#ifdef DEBUG_BINARY
		for(int a=0; a<nread; a++){
			for(int b=7; b>=0; b--){
				printf("%d",(msg[a]>>b)%2);
			}
			printf(" ");
		}
		printf(" || ");
	#endif
	if(!validHeader(msg)){
		printf("invalid header\n");
		exit(-1);
	}
	if(msg[2]>0){
		
		if((nread=read(serialFile, buffer, msg[2]))!=msg[2]){
			printf("error reading data: %d/%d\n", nread, msg[2]);
			exit(-1);
		}
		#ifdef DEBUG_BINARY
			for(int a=0; a<nread; a++){
				for(int b=7; b>=0; b--){
					printf("%d",(buffer[a]>>b)%2);
				}
				printf(" ");
			}
			printf("\n");
		#endif
		if(!validData(buffer, msg[2],msg[3], msg[4])){
			printf("invalid data\n");
			exit(-1);
		}
	}/*else{
		if((nread=read(serialFile, buffer, 1))!=1){
			printf("error reading end: %d/%d\n", nread, msg[2]);
			exit(-1);
		}
	}*/
	
	if(msg[2]!=0){
		memcpy(data, buffer+1, msg[2]);
	}
	printf("%d\n", msg[1]);
	return msg[1];
}