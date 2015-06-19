#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Sensor.h>
#include <Wire.h>

class{ 
	public:
		typedef Vector<double> DataType;
		void init(){
			Wire.begin();
			writeI2C(CTRL_REG1, 0x1F);    // Turn on all axes, disable power down
			writeI2C(CTRL_REG3, 0x08);    // Enable control ready signal
			writeI2C(CTRL_REG4, 0x80);    // Set scale (500 deg/sec)
			writeI2C(CTRL_REG2, 0x09);
		}
		DataType read(){
			//read the top byte, move it over, and read the bottom byte
			int x,y,z;
			x = ((readI2C(0x29) << 8) | readI2C(0x28));
			y = ((readI2C(0x2B) << 8) | readI2C(0x2A));
			z = ((readI2C(0x2D) << 8) | readI2C(0x2C));

			return {x,y,z};
		}
	protected:
		void writeI2C(char regAddr, char val){
			Wire.beginTransmission(ADDR);
			Wire.write(regAddr);
			Wire.write(val);
			Wire.endTransmission();
		}
		int readI2C (char regAddr){
			Wire.beginTransmission(ADDR);
			Wire.write(regAddr);                // Register address to read
			Wire.endTransmission();             // Terminate request
			Wire.requestFrom(ADDR, 1);          // Read a byte
			while(!Wire.available());           // Wait for receipt
			return(Wire.read());                // Get result
		}
		enum {
			CTRL_REG1 =0x20,
                        CTRL_REG2 =0x21,
                        CTRL_REG3 =0x22,
                        CTRL_REG4 =0x23,
                        ADDR      =105
		};
} Gyro;

#endif
