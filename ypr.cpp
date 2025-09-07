//-------------------------------MPU6050 Accelerometer and Gyroscope C++ library-----------------------------
//Copyright (c) 2019, Alex Mous
//Licensed under the CC BY-NC SA 4.0

//Example code modified by Alan Johnston

#include <MPU6050.h>
//#include <linux/i2c-dev.h>
//#include <i2c/smbus.h>

MPU6050 device(0x68);

int main() {
	float ax, ay, az, gx, gy, gz; //Variables to store the accel, gyro and angle values

//	sleep(1); //Wait for the MPU6050 to stabilize

	int f_dev = open("/dev/i2c-1", O_RDWR); //Open the I2C device file
//	std::cout << f_dev << "\n";
	if (f_dev < 0) { //Catch errors
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Failed to open /dev/i2c-1. Please check that I2C is enabled with raspi-config\n"; //Print error message
		std::cout << "0.0 0.0 0.0\n";
		return(-1);	
	}	
	
	int status = ioctl(f_dev, I2C_SLAVE, 0x68); //Set the I2C bus to use the correct address
//	std::cout << status<< "\n";
	if (status < 0) {
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Could not get I2C bus with 0x68 address. Please confirm that this address is correct\n"; //Print error message
		std::cout << "0.0 0.0 0.0\n";
		return(-1);	
	}

	if (i2c_smbus_read_byte_data(f_dev, 0x43) < 0) {
//		std::cout << "ERR (MPU6050.cpp:MPU6050()): Could not read I2C bus with 0x68 address. Please confirm that this address is correct\n"; //Print error message
		std::cout << "0.0 0.0 0.0\n";
		return(-1);			
	}

  	device.calc_yaw = true;

		device.getAngle(0, &gx);
		device.getAngle(1, &gy);
		device.getAngle(2, &gz);
//		std::cout << "Current angle around the roll axis: " << gr << "\n";
//		std::cout << "Current angle around the pitch axis: " << gp << "\n";
//		std::cout << "Current angle around the yaw axis: " << gy << "\n";
//		std::cout << gz << " " <<  gy << " " <<  gx << "\n";

	sleep(1); 

		device.getAngle(0, &gx);
		device.getAngle(1, &gy);
		device.getAngle(2, &gz);

		if (gx < 0) gx += 360;
		if (gy < 0) gy += 360;
		if (gz < 0) gz += 360;
//		std::cout << "Current angle around the roll axis: " << gr << "\n";
//		std::cout << "Current angle around the pitch axis: " << gp << "\n";
//		std::cout << "Current angle around the yaw axis: " << gy << "\n";
		std::cout << gz << " " <<  gy << " " <<  gx << "\n";


//		usleep(1000000); //1 sec
//	}

	return 0;
}
