#include<i2c.hpp>

char *filename = (char *)"/dev/ttyACM0";

void i2c_controller::setup(){
	if ((file_i2c = open(filename, O_RDWR)) < 0) {
		i2c_saveError((char *)"Error opening Serial");
		i2c_saveError(filename);
		exit(-1);
	}
	usleep(1000);
}

