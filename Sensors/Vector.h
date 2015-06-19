#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#include <Arduino.h>

template <class T>
struct Vector {
	T x,y,z;
	typedef Vector<T> type;

	type operator+(type a){ return {x+a.x,y+a.y,z+a.z}; }
	type operator-(type a){ return {x-a.x,y-a.y,z-a.z}; }
	type operator*(type a){ return {x*a.x,y*a.y,z*a.z}; }
	type operator/(type a){ return {x/a.x,y/a.y,z/a.z}; }

	type operator+(T a){ return {x+a,y+a,z+a}; }
	type operator-(T a){ return {x-a,y-a,z-a}; }
	type operator*(T a){ return {x*a,y*a,z*a}; }
	type operator/(T a){ return {x/a,y/a,z/a}; }

	type cross(type a){return {y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x}; }
	T    dot  (type a){return x*a.x + y*a.y + z*a.z;}
	T    mag  ()      {return sqrt(x*x + y*y + z*z);}
	type dir  ()      {return (*this)/mag();}
	
	void print(){
		Serial.print(x);
		Serial.print(", ");
		Serial.print(y);
		Serial.print(", ");
		Serial.println(z);
	}
	
	template <class S>
	Vector<S> as(){return {x,y,z};}
};


template <class T>
struct Matrix {
	typedef Matrix<T> type;
	typedef Vector<T> vec;
	//rows
	vec a,b,c;

	vec  operator*(vec  v){ return {a.dot(v),b.dot(v),c.dot(v)}; }
	type inverse(){return {{det(b.y,b.z,c.y,c.z),det(a.z,a.y,c.z,c.y),det(a.y,a.z,b.y,b.z)},
	                       {det(b.z,b.x,c.z,c.x),det(a.x,a.z,c.x,c.z),det(a.z,a.x,b.z,b.x)},
	                       {det(b.x,b.y,c.x,c.y),det(a.y,a.x,c.y,c.x),det(a.x,a.y,b.x,b.y)}
		};
	}
	template <class S>
	S det(S a1, S a2, S b1, S b2){ return a1*b2-b1*a2; }
};

template <class T>
Matrix<T> columnMatrix(Vector<T> a, Vector<T> b, Vector<T> c){
	return Matrix<T>{{a.x,b.x,c.x},{a.y,b.y,c.y},{a.z,b.z,c.z}};
}

#endif
