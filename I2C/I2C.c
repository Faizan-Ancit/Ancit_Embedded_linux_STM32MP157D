#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>				//Needed for I2C port
#include <linux/i2c-dev.h>			//Needed for I2C port

int main()
{

int file_i2c; \\ to read/write I2C bus
int length; \\
unsigned char buffer[60] = {0}; \\ I2C packets
int addr = 0x5a;          //<<<<<The I2C address of the slave, need to be CHECKED !!!!!
char *filename = (char*)"/dev/i2c-1";




\\\\\\\\\\\\\\\\\\\\\\\\\ ADXL345 Configuration \\\\\\\\\\\\\\\\\\\\\\\\\\

\\\\\\\\\\\\ Register 0x2C - Power Mode and Data Transfer Rate \\\\\\\\\\\

int pm_dtr = 0x2C;
int pm_dtr_data = 0x0A; \\ Normal Mode ; Output Data Rate = 100


\\\\\\\\\\\\ Register 0x31 - Data Format Register \\\\\\\\\\\\\\\\\\

\* read register 0x31, then right justified and , default 10bit resolution +/-2 measurement \\\\ and written back *\

int dfr_reg = 0x31;
int dfr_data = 0x00;

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


\\\\\\\\\\\\ Power Saving Register - 0x2D \\\\\\\\\\\\\\\\\\\\\\\\

int psr_reg = 0x2D;
int psr_data = 0x0A;

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

\\\\\\\\\\\\\\\\\\\\\\\\\\ OPENING I2C BUS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
\\ Changing to I2C Address of ADXL345
if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


\\ Uncomment the following later

\*
\\\\\\\\\\\\\\\\\\\\\ WRITING THE ADDRESS TO THE I2C BUS \\\\\\\\\\\\\\\\\\\\\\

\\\\\\\\\\ Writing to the power mode register \\\\\\\\\\\\\\\\

buffer[0] = pm_dtr;
length = 1;
if (write(file_i2c, buffer, length) != length)	
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
else
	{
		printf("Write Successful");
	}
*\

return 0;

}




















