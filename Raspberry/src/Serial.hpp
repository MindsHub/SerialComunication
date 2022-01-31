#ifndef SERIAL_WRAPPER
#define SERIAL_WRAPPER

#include <cstring>
#include <cerrno>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>

#ifdef DEBUG
	#define DEBUG_LOGGER(...) printf(__VA_ARGS__)
#else
	#define DEBUG_LOGGER(...)
#endif

#define I2C_BUF_SIZE 20
#define TRYES 10

enum {idle, writing, toStart, executing};

class SerialController{
	public:
		SerialController(char * filename, int baudRate=115200);
		SerialController();
		~SerialController();
		
		void reload();
		void setup(char * filename, int baudRate=115200);

		void setControlMode(bool parity=false, bool twoStopBits=false, char bitsPerByte=8, bool flowControl= false); // true/false, true/false, 5/6/7/8, true/false
		void setLocalMode(bool canonical=false, bool echoInput=false, bool signalChars=false);
		void setInputMode(bool sftwFlowControl=false, bool specialHandling=false);
		void setOutputMode(bool specialInterpolation=false);
		void setTimeout(int timeout=10);//input in decisecondi
		void setBaudRate(int baudRate=115200);

		int serialFile=-1;
	private:
		char * filename=NULL;
		int baudRate;
		struct termios tty; //settings for serial
};

class SerialDevice{
	public:
		SerialDevice(char * filename, int baudRate=115200);
		void reload();
		void delivery(unsigned char* data, unsigned char size);
		unsigned char delivery(unsigned char* data, unsigned char size, unsigned char **out);
		
	private:
		SerialController serial;
		int serialFile;
		unsigned char addr;
		unsigned char inBuffer[1000];
		unsigned char outBuffer[4];
		unsigned char result[I2C_BUF_SIZE];
		unsigned char readed=0; //lo so che è sbagliato, ma rende l'idea e non da problemi con la f read
		bool inError=false;
		
		

		bool isValid(unsigned char reg, unsigned char val);
		
		unsigned char readData(unsigned char reg, unsigned char val);
		unsigned char sendData();
		
		bool writeComand(unsigned char reg, unsigned char val);
		unsigned char readComand(unsigned char reg);
		unsigned char readReturn(unsigned char reg);
		bool writeStatus(unsigned char val);
		unsigned char readStatus();
		
		
};

//void setup();
void i2c_saveError(char*);
//void i2c_delivery(unsigned char *, unsigned char);

#endif