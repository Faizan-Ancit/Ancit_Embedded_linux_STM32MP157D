#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>				//Needed for I2C port
#include <linux/i2c-dev.h>			//Needed for I2C port
#include <time.h>

//\\\\\\\\\\\\\\\\\\\\\ CODE FOR CONFIGURATION OF I2C FOR ADXL345 \\\\\\\\\\\\\\\\\\\\

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

int main()
{

int file_i2c; //\\ to read/write I2C bus
int length; 
unsigned char buffer[60] = {0}; //\\ I2C write packets
int x_read_buffer[60] = {0}; // I2C read for x-axis
int addr = 0x53;          //<<<<<The I2C address of the slave, need to be CHECKED !!!!!
char *filename = (char*)"/dev/i2c-1";




//\\\\\\\\\\\\\\\\\\\\\\\\\ ADXL345 Configuration \\\\\\\\\\\\\\\\\\\\\\\\\\

//\\\\\\\\\\\\ Register 0x2C - Power Mode and Data Transfer Rate \\\\\\\\\\\

int pm_dtr = 0x2C;
int pm_dtr_data = 0x0A; //\\ Normal Mode ; Output Data Rate = 100


//\\\\\\\\\\\\ Register 0x31 - Data Format Register \\\\\\\\\\\\\\\\\\

//\* read register 0x31, then right justified and , default 10bit resolution +/-2 measurement \\\\ and written back *\

int dfr_reg = 0x31;
int dfr_data = 0x00;

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


//\\\\\\\\\\\\ Power Saving Register - 0x2D \\\\\\\\\\\\\\\\\\\\\\\\

int psr_reg = 0x2D;
int psr_data = 0x0A;

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


///////////////// Read Registers ////////////////////////////////////

int x_reg_a = 0x32;

////////////////////////////////////////////////////////////////////

/**/





//\\\\\\\\\\\\\\\\\\\\\\\\\\ OPENING I2C BUS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\



	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return 0;
	}
//\\ Changing to I2C Address of ADXL345
if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return 0;
	}
	
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


//\\ Uncomment the following later


//\\\\\\\\\\\\\\\\\\\\\ WRITING THE ADDRESS TO THE I2C BUS \\\\\\\\\\\\\\\\\\\\\\

//\\\\\\\\\\ Writing to the power mode register \\\\\\\\\\\\\\\\

buffer[0] = pm_dtr;
buffer[1] = pm_dtr_data;
length = 2;
if (write(file_i2c, buffer, length) != length)	
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
else
	{
		printf("Power Mode Register configured.\n");
	}


//\\\\\\\\\\\\\\\\\\\ Configuring the Data Format Register \\\\\\\\\\\\\

buffer[0] = dfr_reg;
buffer[1] = dfr_data;
length = 2;
if (write(file_i2c, buffer, length) != length)	
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
else
	{
		printf("Data Format Register configured successfully.\n");
	}
	
//\\\\\\\\\\\\\\\\\\\ Configuring Power Saving Register \\\\\\\\\\\\\\\\\\\\

buffer[0] = psr_reg;
buffer[1] = psr_data;
length = 2;
if (write(file_i2c, buffer, length) != length)	
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
else
	{
		printf("Power Saving Register configured sucessfully.\n");
	}


 /*Introduce a while loop where the data from x-axis is read continuously.*/


while(1){

	// Write to register 0x32 which is the read register for x-axis
	
	buffer[0] = x_reg_a;
	length = 1;
	if (write(file_i2c, buffer, length) != length)	
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the x-axis read register\n");
		return 0;
	}
	else
	{
		printf("Write to x-axis read register successful.\n");
	}
	if (read(file_i2c, x_read_buffer, length) != length) {
		/*Error Handling*/
		printf("Unable to read data from x-axis.\n Fix your code!!");
	}
	else
	{
		printf("X-axis acceleration raw data: %d",x_read_buffer[0]);
	}
	
	delay(2);
}













return 0;

}

