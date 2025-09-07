//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

//Example code modified by Alan Johnston

#include <MPU6050.h>
//#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>

MPU6050 device(0x68);
float ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
float ax, ay, az, gx, gy, gz; //Variables to store the accel, gyro and angle values

void calc_store_offsets();

int main(int argc, char * argv[]) {

	float ax, ay, az, gx, gy, gz; //Variables to store the accel, gyro and angle values

//	sleep(1); //Wait for the MPU6050 to stabilize

	int f_dev = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
//	std::cout << f_dev << "\n";
	if (f_dev < 0) { //Catch errors
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config\n"; //Print error message
		std::cout << "0.0 0.0 0.0 0.0 0.0 0.0\n";
		return(-1);	
	}	
	
	int status = ioctl(f_dev, I2C_SLAVE, 0x68); //Set the I2C bus to use the correct address
//	std::cout << status<< "\n";
	if (status < 0) {
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Could not get I2C bus with 0x68 address. Please confirm that this address is correct\n"; //Print error message
		std::cout << "0.0 0.0 0.0 0.0 0.0 0.0\n";
		return(-1);	
	}

	if (i2c_smbus_read_byte_data(f_dev, 0x43) < 0) {
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Could not read I2C bus with 0x68 address. Please confirm that this address is correct\n"; //Print error message
		std::cout << "0.0 0.0 0.0 0.0 0.0 0.0\n";
		return(-1);			
	}
/*
	//Calculate the offsets
	std::cout << "Calculating the offsets...\n    Please keep the accelerometer level and still\n    This could take a couple of minutes...";
	device.getOffsets(&ax, &ay, &az, &gr, &gp, &gy);
	std::cout << "Gyroscope R,P,Y: " << gr << "," << gp << "," << gy << "\nAccelerometer X,Y,Z: " << ax << "," << ay << "," << az << "\n";
*/

//	for (int i = 0; i < 40; i++) {

      	//Get the current gyroscope values
  	device.getGyro(&gx, &gy, &gz);
//  	std::cout << "Gyroscope Readings: X: " << gr << ", Y: " << gp << ", Z: " << gy << "\n";
  	device.getAccel(&ax, &ay, &az);
//  	std::cout << "Accelerometer Readings: X: " << ax << ", Y: " << ay << ", Z: " << az << "\n";    

	if (argc > 1) { // save offsets

		calc_store_offsets();
		
	 } else {  // read offsets
		
		FILE *offset_file = fopen("/home/pi/MPU6050-C-CPP-Library-for-Raspberry-Pi/offsets", "r");
		if (offset_file == NULL) {
			
    		calc_store_offsets();
			
		} else {
			
			fscanf(offset_file, "%f %f %f %f %f %f", &gx_offset, &gy_offset, &gz_offset, &ax_offset, &ay_offset, &az_offset);
			fclose(offset_file);
		}
	}

	std::cout << (gx + gx_offset) << " " << (gy + gy_offset) << " " << (gz + gz_offset) << " ";

  	std::cout << (ax + ax_offset) << " " << (ay + ay_offset) << " " << (az + az_offset) << "\n";    

	return 0;
}

void calc_store_offsets() {

		gx_offset = -1.0 * gx;
		gy_offset = -1.0 * gy;
		gz_offset = -1.0 * gz;		
		ax_offset = -1.0 * ax;
		ay_offset = -1.0 * ay;
		az_offset = 1.0 - az;

		FILE *offset_file = fopen("/home/pi/MPU6050-C-CPP-Library-for-Raspberry-Pi/offsets", "w");
    	fprintf(offset_file, "%f %f %f %f %f %f", gx_offset, gy_offset, gz_offset, ax_offset, ay_offset, az_offset);
    	fclose(offset_file);
}
