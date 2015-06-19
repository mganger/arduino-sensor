//This is an example file
#include <Accel.h>

void setup()
	Accel.init();
}

void loop(){
	Accel.read().print();
	delay(300);
}
