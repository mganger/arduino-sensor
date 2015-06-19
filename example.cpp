//This is an example file
#include <Accel.h>
#include <Gyro.h>
#include <Magnet.h>
#include <Pressure.h>

void setup()
	Accel      .init();
	Gyro       .init();
	Magnet     .init()
	Pressure   .init();
	Temperature.init();
}

void loop(){
	Accel      .read().print();
	Gyro       .read().print();
	Magnet     .read().print();
	Pressure   .read().print();
	Temperature.read().print();
	delay(300);
}
