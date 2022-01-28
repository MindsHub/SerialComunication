#include<i2c.hpp>

void i2c_device::setup(){
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
		i2c_saveError((char *)"Can't find Arduino on i2c");
		exit(-1);
	} 
	usleep(1000);
}
void i2c_device::setup(i2c_controller inp){
	file_i2c=inp.file_i2c;
	outBuffer[3]=255;
	setup();
}
bool i2c_device::isValid(unsigned char reg, unsigned char val){
	return (inBuffer[0]=='#')&&((unsigned char)(inBuffer[1]+inBuffer[2]+inBuffer[3])==inBuffer[4])&&(inBuffer[1]==reg)&&(inBuffer[2]==val);
}

unsigned char i2c_device::readData(unsigned char reg, unsigned char val){
	
	for(int a=0; a<TRYES; a++){
		readed+=read(file_i2c, inBuffer+readed, 7);
		/*if(readed<7){
			exit(-1);
		}*/
		/*for(int a=0;a<7;a++){
			printf("%d ", inBuffer[a]);
		}
		printf("\n");*/
		while(!isValid(reg, val)&&readed>5){
		  memmove(inBuffer, inBuffer+1, --readed);
		}
		if(isValid(reg, val)&&readed>=5){
			
			DEBUG_LOGGER("\tRead %d\n", inBuffer[3]);
			unsigned char toReturn = inBuffer[3];
			/*if(readed>5){
				readed-=5;
				memmove(inBuffer, inBuffer+5, readed);
			}else{*/
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

unsigned char i2c_device::sendData(){
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

bool i2c_device::writeComand(unsigned char reg, unsigned char val){
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

unsigned char i2c_device::readComand(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=I2C_BUF_SIZE;
		outBuffer[1]=reg;
		return sendData();
	}
	return 0;
}

unsigned char i2c_device::readReturn(unsigned char reg){
	if(reg<I2C_BUF_SIZE){
		outBuffer[0]=I2C_BUF_SIZE+1;
		outBuffer[1]=reg;
		return sendData();
	}
	return 0;
}

bool i2c_device::writeStatus(unsigned char val){
	outBuffer[0]=I2C_BUF_SIZE+2;
	outBuffer[1]=val;
	for(int a=0; a<10; a++){
		if(sendData()==val)
			return true;
	}
	return false;
}

unsigned char i2c_device::readStatus(){
	outBuffer[0]=I2C_BUF_SIZE+3;
	outBuffer[1]=0;
	return sendData();
}

unsigned char i2c_device::delivery(unsigned char* data, unsigned char size, unsigned char **out){
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
		/*for(int a=0;a<result[1]+2;a++){
			printf("%d:", result[a]);
		}
		printf("\n");*/
		if(parity!=result[0]){
			i2c_saveError((char *)"qualcosa è andato storto\n");
			//exit(-1);
		}else{
			*out=result+2;
			return result[1];
		}
	}
}

void i2c_device::delivery(unsigned char* data, unsigned char size){
	unsigned char *out;
	delivery(data, size, &out);//forse è meglio fare 2 procedure completamente diverse? oppure controllo sempre di non aver beccato un errore?
}
