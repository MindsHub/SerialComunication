#include<Serial.hpp>

SerialController::SerialController(char * filename, int baudRate){
	serialFile = open(filename, O_RDWR);

	// Check for errors
	if (serialFile < 0) {
		printf("Error %i from open file %s: %s\n", errno, filename, strerror(errno));
	}

	setControlMode();
	setLocalMode();
	setInputMode();
    setOutputMode();
    setTimeout(10);
    setBaudRate(baudRate);
}

void SerialController::setControlMode(bool parity, bool twoStopBits, char bitsPerByte, bool flowControl){ // true/false, true/false, 5/6/7/8, true/false
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

void SerialController::setLocalMode(bool canonical, bool echoInput, bool parseSignalChars){
	if(canonical)//ricevere i dati riga per riga e ignorando gli invii
		tty.c_lflag |= ICANON;
	else
		tty.c_lflag &= ~ICANON;

	if(echoInput){
		tty.c_lflag |= ECHO; // Disable echo
		tty.c_lflag |= ECHOE; // Disable erasure
		tty.c_lflag |= ECHONL; // Disable new-line echo
	}else{
		tty.c_lflag &= ~ECHO; // Disable echo
		tty.c_lflag &= ~ECHOE; // Disable erasure
		tty.c_lflag &= ~ECHONL; // Disable new-line echo
	}

	if(parseSignalChars) //scegliere se interpretare INTR QUIT e SUSP
		tty.c_lflag |= ISIG;
	else
		tty.c_lflag &= ~ISIG;
}

void SerialController::setInputMode(bool sftwFlowControl, bool specialHandling){
    if(sftwFlowControl)
        tty.c_iflag |= (IXON | IXOFF | IXANY);
    else
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    if(specialHandling)
        tty.c_iflag |= (IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    else
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
}

void SerialController::setOutputMode(bool specialInterpolation){
    if(specialInterpolation){
        tty.c_oflag |= OPOST;
        tty.c_oflag |= ONLCR;
    }else{
        tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
        tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    }
}

void SerialController::setTimeout(int timeout){//input in decisecondi
    tty.c_cc[VMIN]=0;
    tty.c_cc[VTIME]=timeout;
}

void SerialController::setBaudRate(int baudRate){ //non so cosa B0 faccia
    switch(baudRate){//B0,  B50,  B75,  B110,  B134,  B150,  B200, B300, B600, B1200, B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800
        case 0:
           cfsetispeed(&tty, B0);
        break;
        case 50:
           cfsetispeed(&tty, B50);
        break;
        case 75:
           cfsetispeed(&tty, B75);
        break;
        case 110:
           cfsetispeed(&tty, B110);
        break;
        case 134:
           cfsetispeed(&tty, B134);
        break;
        case 150:
           cfsetispeed(&tty, B150);
        break;
        case 200:
           cfsetispeed(&tty, B200);
        break;
        case 300:
           cfsetispeed(&tty, B300);
        break;
        case 600:
           cfsetispeed(&tty, B600);
        break;
        case 1200:
           cfsetispeed(&tty, B1200);
        break;
        case 1800:
           cfsetispeed(&tty, B1800);
        break;
        case 2400:
           cfsetispeed(&tty, B2400);
        break;
        case 4800:
           cfsetispeed(&tty, B4800);
        break;
        case 9600:
           cfsetispeed(&tty, B9600);
        break;
        case 19200:
           cfsetispeed(&tty, B19200);
        break;
        case 38400:
           cfsetispeed(&tty, B38400);
        break;
        case 57600:
           cfsetispeed(&tty, B57600);
        break;
        case 115200:
           cfsetispeed(&tty, B115200);
        break;
        case 230400:
           cfsetispeed(&tty, B230400);
        break;
        case 460800:
           cfsetispeed(&tty, B460800);
        break;
        default:
            printf("not a standard baudRate\n");
            exit(-1);
    }
}