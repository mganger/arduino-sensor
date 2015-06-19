#ifndef SENSOR_H
#define SENSOR_H

#include <Vector.h>

template <typename Type>
class Sensor {
		typedef typename Type::DataType DataType;
	public:
		Sensor(){ }
		DataType read(){
			if(!isInitialized) static_cast<Type *>(this)->init(), isInitialized = true;
			return static_cast<Type *>(this)->readDataFromSensor();
		}
		void calibrate(){
			DataType sum;
			for(int i = 0; i < 1000; i++) sum = sum + read();
			calibration = sum/1000.0;
		}
		DataType readCal(){
			return read() - calibration;
		}
	protected:
		DataType calibration;
		static bool isInitialized;
};

template <typename Type>
bool Sensor<Type>::isInitialized = false;

#endif
