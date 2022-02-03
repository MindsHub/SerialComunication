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
	if(data[0]!=17||data[6]!=19){
		printf("invalid header start/end\n");
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
	if(data[0]!=17||data[dataSize+1]!=19){
		printf("invalid data start/end\n");
		return false;
	}
	byte checksum=0;
	byte pChecksum=0;
	for(int a=1; a<=dataSize; a++){
		checksum+=data[a-1];
		pChecksum+=a*data[a-1];
	}
	if(checksum!=dataChecksum||pChecksum!=progressiveChecksum){
		printf("invalid data checksum\n");
		return false;
	}
	return true;
}

void SerialDevice::send(byte cmd, byte dataSize, byte* data){//XON-int cmd-int dataSize-int data checksum - int progressive- int index checksum - XOFF
	byte msg[7];
	byte dataChecksum=0;
	byte progressiveChecksum=0;
	if(dataSize!=0){
		for(int a=1; a<=dataSize; a++){
			dataChecksum+=data[a-1];
			progressiveChecksum+=a*data[a-1];
		}
	}
	

	msg[0] = 17;//XON
	msg[1] = cmd;
	msg[2] = dataSize;
	msg[3] = dataChecksum;
	msg[4] = progressiveChecksum;
	byte indexChecksum = cmd+dataSize+dataChecksum+progressiveChecksum;
	msg[5] = indexChecksum;
	msg[6] = 19; //XOFF
	write(serialFile, msg, 6);
	if(dataSize>1)
		write(serialFile, data, dataSize);
	write(serialFile, msg+6, 1);//file end
}

void SerialDevice::receive(byte* data){//XON-int cmd-int dataSize-int data checksum - int progressive- int index checksum - XOFF
	byte msg[8];
	byte buffer[258];
	int nread;
	if((nread=read(serialFile, msg, 8))!=8){
		printf("error reading header: %d/8\n", nread);
		exit(-1);
	}
	if(!validHeader(msg)){
		printf("invalid header\n");
		exit(-1);
	}
	
	if((nread=read(serialFile, buffer, msg[2]+2))!=msg[2]+2){
		printf("error reading data: %d/%d\n", nread, msg[2]);
		exit(-1);
	}

	if(!validData(buffer, msg[2],msg[3], msg[4])){
		printf("invalid data\n");
		exit(-1);
	}
	memcpy(data, buffer+1, msg[2]);
}