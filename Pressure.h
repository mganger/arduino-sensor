#ifndef PRESSURE_H
#define PRESSURE_H

//Magnetic sensor, inherits from the sensor class

#include <Sensor.h>
#include <Wire.h>

class {
	public:
		void init(){
			if(isInitialized) return;

			//mode (this was originally passed as an argument)
			char mode = BMP085_ULTRAHIGHRES;

			//run init code
			if (mode > BMP085_ULTRAHIGHRES) mode = BMP085_ULTRAHIGHRES;
			oversampling = mode;
			
			if (read8(0xD0) != 0x55) return;
			
			/* read calibration data */
			ac1 = read16(BMP085_CAL_AC1);
			ac2 = read16(BMP085_CAL_AC2);
			ac3 = read16(BMP085_CAL_AC3);
			ac4 = read16(BMP085_CAL_AC4);
			ac5 = read16(BMP085_CAL_AC5);
			ac6 = read16(BMP085_CAL_AC6);
			
			b1 = read16(BMP085_CAL_B1);
			b2 = read16(BMP085_CAL_B2);
			
			mb = read16(BMP085_CAL_MB);
			mc = read16(BMP085_CAL_MC);
			md = read16(BMP085_CAL_MD);
		}
	
		bool isInitialized = false;
		long int computeB5(long UT){
			long X1 = (UT - (long)ac6) * ((long)ac5) >> 15;
			long X2 = ((long)mc << 11) / (X1+(long)md);
			return X1 + X2;
		}
		unsigned int readRawTemperature(){
			write8(BMP085_CONTROL, BMP085_READTEMPCMD);
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!WHAT?!!!!!!!!
			delay(5);
			return read16(BMP085_TEMPDATA);
		}
		double readTemperature(){
			long int UT = readRawTemperature();
			long int B5 = computeB5(UT);
			return (B5+8) >> 4;
		}
		unsigned long int readRawPressure(){
			write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6));
			
			if (oversampling == BMP085_ULTRALOWPOWER) delay(5);
			else if (oversampling == BMP085_STANDARD) delay(8);
			else if (oversampling == BMP085_HIGHRES) delay(14);
			else delay(26);
			
			unsigned long raw;
			raw = read16(BMP085_PRESSUREDATA);
			raw <<= 8;
			raw |= read8(BMP085_PRESSUREDATA+2);
			raw >>= (8 - oversampling);
			return raw;
		}
		double readPressure(){
			long int UT, UP, B3, B5, B6, X1, X2, X3, p;
			unsigned long int B4, B7;
			
			UT = readRawTemperature();
			UP = readRawPressure();
			B5 = computeB5(UT);
			
			// do pressure calcs (equations from BMP085 Datasheet)
			B6 = B5 - 4000;
			X1 = ((long)b2 * ( (B6 * B6)>>12 )) >> 11;
			X2 = ((long)ac2 * B6) >> 11;
			X3 = X1 + X2;
			B3 = ((((long)ac1*4 + X3) << oversampling) + 2) / 4;
			
			X1 = ((long)ac3 * B6) >> 13;
			X2 = ((long)b1 * ((B6 * B6) >> 12)) >> 16;
			X3 = ((X1 + X2) + 2) >> 2;
			B4 = ((unsigned long)ac4 * (unsigned long)(X3 + 32768)) >> 15;
			B7 = ((unsigned long)UP - B3) * (unsigned long)(50000UL >> oversampling );
			
			if(B7 < 0x80000000) p = (B7 * 2) / B4;
			else p = (B7 / B4) * 2;

			X1 = (p >> 8) * (p >> 8);
			X1 = (X1 * 3038) >> 16;
			X2 = (-7357 * p) >> 16;
			
			return p + ((X1 + X2 + (int32_t)3791)>>4);
		}

	protected:
		char read8(char a){
			Wire.beginTransmission(BMP085_I2CADDR);
			//sends register address to read from
			Wire.write(a);				
			Wire.endTransmission();
			
			Wire.beginTransmission(BMP085_I2CADDR);
			//send data n-bytes read
			Wire.requestFrom(BMP085_I2CADDR, 1);
			char ret = Wire.read();
			Wire.endTransmission();
			
			return ret;
		}
		unsigned int read16(char a){
			Wire.beginTransmission(BMP085_I2CADDR);
			//sends register address to read from
			Wire.write(a);
			Wire.endTransmission();
			
			Wire.beginTransmission(BMP085_I2CADDR);
			//send data n-bytes read
			Wire.requestFrom(BMP085_I2CADDR, 2);
			unsigned int ret = Wire.read();
			ret <<= 8;
			ret |= Wire.read();
			Wire.endTransmission();
			
			return ret;
		}
		void write8(char a, char d){
			Wire.beginTransmission(BMP085_I2CADDR);
			//sends register address to read from
			Wire.write(a);
			//write data
			Wire.write(d); 
			Wire.endTransmission();
		}

		//calibrations (straight from data sheet, no idea what they mean)
		char oversampling;
		int ac1, ac2, ac3, b1, b2, mb, mc, md;
		unsigned int ac4, ac5, ac6;
		enum {
			BMP085_CAL_AC1		=0xAA,	// R	Calibration data (16 bits)
			BMP085_CAL_AC2		=0xAC,	// R	Calibration data (16 bits)
			BMP085_CAL_AC3		=0xAE,	// R	Calibration data (16 bits)
			BMP085_CAL_AC4		=0xB0,	// R	Calibration data (16 bits)
			BMP085_CAL_AC5		=0xB2,	// R	Calibration data (16 bits)
			BMP085_CAL_AC6		=0xB4,	// R	Calibration data (16 bits)
			BMP085_CAL_B1		=0xB6,	// R	Calibration data (16 bits)
			BMP085_CAL_B2		=0xB8,	// R	Calibration data (16 bits)
			BMP085_CAL_MB		=0xBA,	// R	Calibration data (16 bits)
			BMP085_CAL_MC		=0xBC,	// R	Calibration data (16 bits)
			BMP085_CAL_MD		=0xBE,	// R	Calibration data (16 bits)

			BMP085_CONTROL		=0xF4, 
			BMP085_TEMPDATA		=0xF6,
			BMP085_PRESSUREDATA	=0xF6,
			BMP085_READTEMPCMD	=0x2E,
			BMP085_READPRESSURECMD	=0x34,

			BMP085_I2CADDR          =0x77,
					      
			BMP085_ULTRALOWPOWER    =0,
			BMP085_STANDARD         =1,
			BMP085_HIGHRES          =2,
			BMP085_ULTRAHIGHRES     =3
		};
} PressTemp;


class {
	public:
		typedef double DataType;
		DataType read(){
			return PressTemp.readPressure();
		}
		void init(){PressTemp.init();}
} Pressure;


/////////////////////////////////////////////////////

class {
	public:
		typedef double DataType;
		DataType read(){
			return PressTemp.readTemperature();
		}
		void init(){PressTemp.init();}
} Temperature;


#endif
