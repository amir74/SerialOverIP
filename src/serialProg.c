#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

//serial port programming libs
#include <fcntl.h> //file controls 
#include <errno.h> //Error integer and strerror func
#include <termios.h> //POSIX terminal control
#include <unistd.h> //file I/O

//preproc
#define unsigned int uint;
#define true 1;
#define false 0;
typedef struct termios term;
//TCP port whitelist
//Placeholder for now since i dont have much of an idea of what 
//it would looks like in action
int TCPList[] = {4001, 4018, 4200, 4840, 6005, 6464, 4090, 4093};

int parseArgs(int argc, char *argv[], uint *configs)
{
    int count;

    if(argc==1)
    {
        printf("\n USAGE: TCPport Baudrate Data Parity StopBits FlowControl \n");
    }
    else if(argc == 7)
    {
      for(count = 1; count < argc; count++)
        {
            static int i = 0;
            printf("argv[%d] = %s\n", count, argv[count]);
            configs[i] = atoi(argv[count]);
            i++;
        }
    }
    else
    {
        printf("\n Invalid number of arguments.\n");
        printf("\n USAGE: TCPport Baudrate Data Parity StopBits FlowControl \n");

    }
    return 0;
}

int verifyTCP(int port)
{
    //basic verification to see if we can connect to port 
    // if number of possible TCP ports is high should be looked into to find a better solution
    int count; 
    for ( count = 0; i < sizeof(TCPList) ; i++)
    {
        if (TCPList[count] == port)
        {
            return true; //port was accepted
        }
        else
        {
            return false; //not found 
        }
    }
}

int SetTerminal(term SerialTerminal, int SerialPort, uint configs[])
{

    //Parity
    switch(configs[3])
    {
        case 'NONE':
        {
            //disables parity
            SerialTerminal.c_cflag &= ~PARENB;
            break;
        }
        case 'ODD':
        {
            //Odd parity
            SerialTerminal.c_cflag |= PARENB;
            break;
        }
        case 'EVEN':
        {
            SerialTerminal.c_cflag &= ~PARODD;
        }
    }

    //Stop Bits
    switch(configs[4])
    {
        case 1:
        {
            //1 stop bit
            SerialTerminal.c_cflag &= ~CSTOPB;
            break;
        }
        case 2:
        {
            //2stop bits
            SerialTerminal.c_cflag |= CSTOPB;
            break;
        }
    }

    //clear bits that set the data size
    SerialTerminal.c_cflag &= ~CSIZE;

    //Data bits
    switch (configs[2])
    {
        case 5:
        {
            SerialTerminal.c_cflag |= CS5;
            break;
        }
        case 6:
        {
            SerialTerminal.c_cflag |= CS6;
            break;
        }
        case 7:
        {
            //7 bit encoding
            SerialTerminal.c_cflag |= CS7;
            break;
        }
        case 8:
        {
            //8 bit encoding
            SerialTerminal.c_cflag |= CS8;
            break;
        }
    }

    //disable hardware control for now 
    SerialTerminal.c_cflag  &= ~CRTSCTS;

    //Turn on READ and ignore ctrl lines like new line and what not
    SerialTerminal.c_cflag |= CREAD | CLOCAL;

    //disable cannonical mode
    SerialTerminal.c_lflag &= ~ICANON;

    //Disable ECHO
    SerialTerminal.c_lflag &= ~ECHO;

    //Disable Erasure
    SerialTerminal.c_lflag &= ~ECHOE;

    //Disable new-line echo
    SerialTerminal.c_lflag &= ~ECHONL;

    //Disable INTR QUITE AND SUSPEND flags
    SerialTerminal.c_lflag &= ~ISIG;

    //turn off s/w flow ctrl
    SerialTerminal.c_lflag &= ~(IXON | IXOFF | IXANY);

    //disable special handling of special bytes in order to make it transparent
    SerialTerminal.c_lflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    //Disable newline
    SerialTerminal.c_oflag &= ~OPOST;
    SerialTerminal.c_oflag &= ~ONLCR;

    //no blocking
    SerialTerminal.c_cc[VTIME] = 0;
    SerialTerminal.c_cc[VMIN] = 0;


    //INPUT OUTPUT BAUDRATES
    cfsetispeed(&SerialTerminal, 0000017);
    cfsetospeed(&SerialTerminal, 0000017);

    // Write to port and error handling
    if (tcsetattr(SerialPort, TCSAFLUSH, &SerialTerminal)!= 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }

}


int main (int argc, char *argv[])
{

    // change the Serial Port value for every port and create a binary for it 
    // seems a better choice than passing another argument from cmd line 
    // in allowing for finer grain control
    int SerialPort = open("/dev/tty", O_RDWR|O_NDELAY|O_NOCTTY);
    if( SerialPort < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    term SerialTerminal;
    uint configs[5];
    uint *Pconfigs = &configs;

    // parse the input arguments
    parseArgs(argc, argv, Pconfigs);

    //check the tcp port
    //verifyTCP(configs[0]);

    //set port settings
    SetTerminal(SerialTerminal, SerialPort, configs);

    close(SerialPort);

    return 0;
    
}