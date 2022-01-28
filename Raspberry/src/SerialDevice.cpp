#include<Serial.hpp>

SerialDevice::SerialDevice(char * filename){
	serialFile = open(filename, O_RDWR);

	// Check for errors
	if (serialFile < 0) {
		printf("Error %i from open file %s: %s\n", errno, filename, strerror(errno));
	}

	setControlMode();
	
}

void SerialDevice::setControlMode(bool parity=false, bool twoStopBits=false, char bitsPerByte=8, bool flowControl= false){ // true/false, true/false, 5/6/7/8, true/false
	if(parity) //abilita o disabilita il parity bit
		tty.c_cflag |= PARENB;
	else
		tty.c_cflag &= ~PARENB;
	
	if(twoStopBits) //sceglie quanti stop bits usare, se 2 o solo 1
		tty.c_cflag |= CSTOPB;
	else
		tty.c_cflag &= ~CSTOPB;
	
	tty.c_cflag &= ~CSIZE; // azzeramento bit prima dell'assegnamento
	switch(bitsPerByte){
		case 5:
			tty.c_cflag |= CS5;
		break;
		case 6:
			tty.c_cflag |= CS6;
		break;
		case 7:
			tty.c_cflag |= CS7;
		break;
		case 8:
			tty.c_cflag |= CS8;
		break;
		default:
			printf("not a valid bits per byte value\n");
			exit(-1);
		break;
	}

	if(flowControl) //hardware flow control
		tty.c_cflag |= CRTSCTS;
	else
		tty.c_cflag &= ~CRTSCTS;

	tty.c_cflag |= CREAD | CLOCAL; //abilita lettura e ignora ctrl lines
}

bool SerialDevice::isValid(unsigned char reg, unsigned char val){
	return (inBuffer[0]=='#')&&((unsigned char)(inBuffer[1]+inBuffer[2]+inBuffer[3])==inBuffer[4])&&(inBuffer[1]==reg)&&(inBuffer[2]==val);
}
/*
unsigned char SerialDevice::readData(unsigned char reg, unsigned char val){
	
	for(int a=0; a<TRYES; a++){
		readed+=read(file_i2c, inBuffer+readed, 7);
		while(!isValid(reg, val)&&readed>5){
		  memmove(inBuffer, inBuffer+1, --readed);
		}
		if(isValid(reg, val)&&readed>=5){
			
			DEBUG_LOGGER("\tRead %d\n", inBuffer[3]);
			unsigned char toReturn = inBuffer[3];
			readed=0;
			//}
			//printf("%d\n", a);
			inError=false;
			return toReturn;

		}
		usleep(100);
	}
	
	i2c_saveError((char *) "can't read from i2c device");
	I2C_CONTROLLER.setup();
	setup(I2C_CONTROLLER);
	inError=true;
	return 0;
}

unsigned char SerialDevice::sendData(){
	while(true){
		
		outBuffer[2]=outBuffer[0]+ outBuffer[1];
		
		
		for(int a=0; a<TRYES; a++){
			DEBUG_LOGGER("\tSend: %d %d %d\n", outBuffer[0], outBuffer[1], outBuffer[2]);
			if(4 == write(file_i2c, outBuffer, 4)){
				usleep(10);
				unsigned char toReturn = readData(outBuffer[0],outBuffer[1]);
				if(!inError)
					return toReturn;
			}
			usleep(100);
		}
		i2c_saveError((char *) "can't write to i2c device");
		I2C_CONTROLLER.setup();
		setup(I2C_CONTROLLER);
	}
}

bool SerialDevice::writeComand(unsigned char reg, unsigned char val){
	//printf("%d %d\n",reg, val);
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=reg;
		outBuffer[1]=val;
		for(int a=0; a<10; a++){
			if(sendData()==outBuffer[1])
				return true;
		}
	}
	i2c_saveError((char *)"failed write\n");
	return false;
}

unsigned char SerialDevice::readComand(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=I2C_BUF_SIZE;
		outBuffer[1]=reg;
		return sendData();
	}
	return 0;
}

unsigned char SerialDevice::readReturn(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=I2C_BUF_SIZE+1;
		outBuffer[1]=reg;
		return sendData();
	}
	return 0;
}

bool SerialDevice::writeStatus(unsigned char val){
	outBuffer[0]=I2C_BUF_SIZE+2;
	outBuffer[1]=val;
	for(int a=0; a<10; a++){
		if(sendData()==val)
			return true;
	}
	return false;
}

unsigned char SerialDevice::readStatus(){
	outBuffer[0]=I2C_BUF_SIZE+3;
	outBuffer[1]=0;
	return sendData();
}

unsigned char SerialDevice::delivery(unsigned char* data, unsigned char size, unsigned char **out){
	//printf("delivery time\n");
	unsigned char parity=0;
	for(int a=0; a<size; a++){
		//printf("%d %d\n",a+2, data[a]);
		writeComand(a+2, data[a]);
		parity+=data[a];
	}
	//printf("data written\n");
	writeComand(0, parity);
	writeComand(1, size);
	
	writeStatus(toStart);
	
	
	while(readStatus()!=idle) usleep(10);
	
	while(true){//reading
		
		result[0] = readReturn(0);
		result[1] = readReturn(1);
		parity=0;
		for(int a=0;a<result[1];a++){
			result[a+2]=readReturn(a+2);
			parity+=result[a+2];
		}
		if(parity!=result[0]){
			i2c_saveError((char *)"qualcosa è andato storto\n");
			//exit(-1);
		}else{
			*out=result+2;
			return result[1];
		}
	}
}

void SerialDevice::delivery(unsigned char* data, unsigned char size){
	unsigned char *out;
	delivery(data, size, &out);//forse è meglio fare 2 procedure completamente diverse? oppure controllo sempre di non aver beccato un errore?
}
*/