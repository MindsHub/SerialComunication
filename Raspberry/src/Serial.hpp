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



class SerialDevice{
	public:
		SerialDevice(char * filename);
		void delivery(unsigned char* data, unsigned char size);
		unsigned char delivery(unsigned char* data, unsigned char size, unsigned char **out);
		
	private:
		int serialFile;
		struct termios tty; //settings for serial
		unsigned char addr;
		unsigned char inBuffer[1000];
		unsigned char outBuffer[4];
		unsigned char result[I2C_BUF_SIZE];
		unsigned char readed=0; //lo so che è sbagliato, ma rende l'idea e non da problemi con la f read
		bool inError=false;
		
		void setControlMode(bool parity=false, bool twoStopBits=false, char bitsPerByte=8, bool flowControl= false); // true/false, true/false, 5/6/7/8, true/false


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