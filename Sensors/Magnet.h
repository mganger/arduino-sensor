#ifndef MAGNET_H
#define MAGNET_H

#include <Sensor.h>
#include <Wire.h>

class {
	public:
		typedef Vector<double> DataType;
		void init(){
			Wire.begin();
			Wire.beginTransmission(ADDRESS);
			//select mode register, then continuous measurement mode
			Wire.write(0x02);
			Wire.write(0x00);
			Wire.endTransmission();
		}
		DataType read(){
			//Tell the HMC5883 where to begin reading data
			Wire.beginTransmission(ADDRESS);
			//select register 3, X MSB register
			Wire.write(0x03);
			Wire.endTransmission();

			int x,y,z;
			Wire.requestFrom(ADDRESS, 6);
			if(Wire.available() >= 6 ){
				//x,z,y
				x = (Wire.read() << 8) | (Wire.read());
				y = (Wire.read() << 8) | (Wire.read());
				z = (Wire.read() << 8) | (Wire.read());
			}

			return {x,y,z};
		}
	protected: enum {
		ADDRESS=0x1E //0011110b, I2C 7bit address of HMC5883
	};
} Magnet;


#endif
