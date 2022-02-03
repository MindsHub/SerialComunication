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

typedef unsigned char byte;

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
		void send(byte cmd, byte dataSize, byte* data);
		void receive(byte* data);
	private:
		SerialController serial;
		int serialFile;
		bool validHeader(byte* data);
		bool validData(byte* data, byte dataSize, byte dataChecksum, byte progressiveChecksum);
		
};

//void setup();
void i2c_saveError(char*);
//void i2c_delivery(unsigned char *, unsigned char);

#endif