#ifndef ACCEL_H
#define ACCEL_H

//Accelerometer

#include <Sensor.h>
#include <Wire.h>

class {
	public:
		typedef Vector<double> DataType;
		void init(){
			Wire.begin();
			//Put the ADXL345 into +/-4G range by writing the value
			//0x01 to the DATA_FORMAT register.
			writeTo(DATA_FORMAT, 0x01);
			//Put the ADXL345 into Measurement Mode by writing 0x08
			//to the POWER_CTL register.
			writeTo(POWER_CTL, 0x08);
		}
		DataType read(){
			Wire.beginTransmission(DEVICE);
			Wire.write(DATAX0);
			Wire.endTransmission();


			Wire.beginTransmission(DEVICE);
			Wire.requestFrom(DEVICE, 6);

			//each axis reading comes in 10 bit resolution
			//Least Significant Byte first!!
			int x,y,z;
			if(Wire.available()){
				int tmp = Wire.read();
				x = ((int)Wire.read() << 8) | tmp;
				tmp = Wire.read();
				y = ((int)Wire.read() << 8) | tmp;
				tmp = Wire.read();
				z = ((int)Wire.read() << 8) | tmp;
			}
			Wire.endTransmission();

			return {x,y,z};
		}
	private:
		void writeTo(char address, char value){
			Wire.beginTransmission(DEVICE); 
			Wire.write(address); 
			Wire.write(value); 
			Wire.endTransmission();
		}
		enum {
			DEVICE      =0x53,  //Device address as specified in data sheet 
                        POWER_CTL   =0x2D,  //Power Control Register
                        DATA_FORMAT =0x31,  
                        DATAX0      =0x32,  //X-Axis Data 0
                        DATAX1      =0x33,  //X-Axis Data 1
                        DATAY0      =0x34,  //Y-Axis Data 0
                        DATAY1      =0x35,  //Y-Axis Data 1
                        DATAZ0      =0x36,  //Z-Axis Data 0
                        DATAZ1      =0x37   //Z-Axis Data 1
		};
} Accel;

#endif
