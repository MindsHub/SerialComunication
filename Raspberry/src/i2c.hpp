#ifndef i2c_wrapper
#define i2c_wrapper

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

class i2c_controller{
	public:
		int file_i2c;
		i2c_controller(){
			setup();
		}
		void setup();
		
};

class i2c_device{
	public:
		i2c_device(i2c_controller inp, unsigned char addr){
			this->addr=addr;
			setup(inp);
		}
		void delivery(unsigned char* data, unsigned char size);
		unsigned char delivery(unsigned char* data, unsigned char size, unsigned char **out);
	private:
		int file_i2c;
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
		
		void setup(i2c_controller inp);
		void setup();
		
};

//void setup();
void i2c_saveError(char*);
//void i2c_delivery(unsigned char *, unsigned char);

#define I2C_CONTROLLER __getI2cController()
extern i2c_controller& __getI2cController(void);

#endif
