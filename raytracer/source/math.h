#ifndef RAYTRACER_MATH_H
#define RAYTRACER_MATH_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

template <typename T, int size>
class vec {
public:
	T* arr;

public:
	// constructors
	vec<T, size> (){
		arr = new T[size];
	}
	vec<T, size> (const vec<T,size>& v){
		arr = new T[size];
		for(int i = 0; i < size; i++)
			arr[i] = v.arr[i];
	}
	vec<T, size> &operator=(const vec<T, size>& rhs){
		for(int i = 0; i < size; i++)
			arr[i] = rhs.arr[i];
		return *this;
	}
	vec<T, size> ( vec<T,size>& v){
		arr = new T[size];
		for(int i = 0; i < size; i++)
			arr[i] = v.arr[i];
	}

  
	vec<T, size>(T value){
		arr = new T[size];
		for(int i = 0; i < size; i++)
			arr[i] = value;
	}

	vec<T, size> (T* Tarr) {
		arr = new T[size];
  
		for(int i = 0; i < size; i++)
			arr[i] = Tarr[i];
	}

	vec<T, size> (std::vector<T> Tarr){
		arr = new T[size];
		for(int i = 0; i < size; i++)
			arr[i] = Tarr[i];
	}

	vec<T,size> (T _x, T _y,T _z ) {
		if( size == 3){
			arr = new T[3];
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z;
		}
		else
			throw "error";
	}
	
	vec<T, size> (T _x, T _y, T _z, T _w ) {
		if( size == 4){
			arr = new T[4];
			arr[0] = _x;
			arr[1] = _y;
			arr[2] = _z;
			arr[3] = _w;
		}
		else
			throw "error";

	}

	// deconstructors
	~vec<T, size> (){
		if(arr)
			delete [] arr;
		arr = nullptr;

	}

	// field reach
	T& x() { return arr[0]; }
	T& y() { return arr[1]; }
	T& z() { return arr[2]; }
	T& w() { if(size >=4 )return arr[3]; else throw std::runtime_error("4th element does not exist"); }

	T& r() {  return arr[0];}
	T& g() {  return arr[1];}
	T& b() {  return arr[2];}
    T& a() { if(size >=4 )return arr[3]; else throw std::runtime_error("4th element does not exist"); }

  	T x()const {  return arr[0];}
	T y()const {  return arr[1];}
	T z()const {  return arr[2];}
	T w()const { if(size >=4  )return arr[3]; else throw std::runtime_error("4th element does not exist"); }

	T r()const { return arr[0];}
	T g()const { return arr[1];}
	T b()const { return arr[2];}
    T a()const { if(size >= 4 )return arr[3]; else throw std::runtime_error("4th element does not exist"); }


	// debug && strings
	std::string toString() {
		std::string res = "";
		for(int i = 0; i < size; i++)
			res += std::to_string(arr[i]) + " ";
		return res;
	}  
	
	std::ostream& print(){
		return std::cout << toString() << std::endl;
	}
	


	//// vector to vector operations
	vec<T, size> operator+(const vec<T, size> &rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs.arr[i];
		return tmp;
	}
	vec<T, size> operator-(const vec<T, size> &rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++){
			tmp.arr[i] = arr[i] - rhs.arr[i];
		}
		return tmp;
	}
	vec<T, size> operator+( vec<T, size> &rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs.arr[i];
		return tmp;
	}
	vec<T, size> operator-( vec<T, size> &rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] - rhs.arr[i];
		return tmp;
	}
	vec<T, size> operator*(vec<T, size> & rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i <size; i++)
			tmp.arr[i] = arr[i] * rhs.arr[i];
		return tmp;
	}
	vec<T, size> operator*(const vec<T, size> & rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i <size; i++)
			tmp.arr[i] = arr[i] * rhs.arr[i];
		return tmp;
	}
	

  
  
	//// vector to type operations
	// divide
	vec<T, size> operator/( double divider) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] / divider;
		return tmp;
	}
	vec<T, size> operator/( float divider)const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] / divider;
		return tmp;
	}
	vec<T, size> operator/( int divider) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] / divider;
		return tmp;
	}
	vec<T, size> operator/( uint divider) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] / divider;
		return tmp;
	}


	// multiply
	vec<T, size> operator*( double multiplier) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] * multiplier;
		return tmp;
	}
	vec<T, size> operator*( float multiplier) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] * multiplier;
		return tmp;
	}
	vec<T, size> operator*( int multiplier) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] * multiplier;
		return tmp;
	}
	vec<T, size> operator*( uint multiplier) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] * multiplier;
		return tmp;
	}

	// addition
	vec<T, size> operator+( double rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs;
		return tmp;
	}
	vec<T, size> operator+( float rhs)const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs;
		return tmp;
	}
	vec<T, size> operator+( int rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs;
		return tmp;
	}
	vec<T, size> operator+( uint rhs)const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = arr[i] + rhs;
		return tmp;
	}

	// substraction
	vec<T, size> operator-( double rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i--)
			tmp.arr[i] = arr[i] - rhs;
		return tmp;
	}
	vec<T, size> operator-( float rhs)const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i--)
			tmp.arr[i] = arr[i] - rhs;
		return tmp;
	}
	vec<T, size> operator-( int rhs) const{
		vec<T, size> tmp;
		for(int i = 0; i < size; i--)
			tmp.arr[i] = arr[i] - rhs;
		return tmp;
	}
	vec<T, size> operator-( uint rhs) const{ 
		vec<T, size> tmp;
		for(int i = 0; i < size; i--)
			tmp.arr[i] = arr[i] - rhs;
		return tmp;
	}


	
	// vectoral
	static T dot(const vec<T, size>& lhs, const vec<T, size>& rhs ){
		T sum = 0;
		for(int i = 0; i < size; i++)
			sum += ( lhs.arr[i] * rhs.arr[i] );
		return sum;
	}

	static vec<T, size> cross(const vec<T, size>& lhs, const vec<T, size>& rhs) {
		if(size == 3)
			return {
				lhs.y()*rhs.z() - lhs.z()*rhs.y(),
				lhs.z()*rhs.x() - lhs.x()*rhs.z(),
				lhs.x()*rhs.y() - lhs.y()*rhs.x()				
			};
		else
			throw "NoMatchingFunction 'cross' for 'size !=3'";
	}

	T sqrMagnitude() const{
		T res = 0;
		for(int i = 0; i <size; i ++)
			res += arr[i]*arr[i];
		return res;
	}
	T magnitude() const{
		T res = 0;
		for(int i = 0; i <size; i ++)
			res += arr[i]*arr[i];
		return sqrt(res);
	}

	vec<T,size>& normalize(){
		T res = 0;
		for(int i = 0; i <size; i ++)
			res += arr[i]*arr[i];
		res = sqrt(res);
		for(int i = 0; i <size; i ++)
			arr[i] /= res;
		return *this;
	}
	
	vec<T,size> normalized() {
		vec<T, size> tmp = *this;
		T res = 0;
		for(int i = 0; i <size; i ++)
			res += arr[i]*arr[i];
		res = sqrt(res);
		for(int i = 0; i <size; i ++)
			tmp.arr[i] /= res;
		return tmp;
	}


	vec<unsigned short int, size> round() {
		vec<unsigned short int, size> tmp;
		for(int i = 0; i < size; i++)
			tmp.arr[i] = (int)(arr[i] + (T)0.5);
		return tmp;
	}


	
	void loadFromStream(std::stringstream& stream) {
		for(int i = 0 ; i < size ; i++)
			stream >> arr[i];
	}





	// cout
	friend std::ostream& operator<<( std::ostream& os,  vec<double, 3>& rhs);
	friend std::ostream& operator<<( std::ostream& os,  vec<double, 4>& rhs);
	friend std::ostream& operator<<( std::ostream& os,  vec<float, 3>& rhs);
	friend std::ostream& operator<<( std::ostream& os,  vec<float, 4>& rhs);
	friend std::ostream& operator<<( std::ostream& os,  vec<int, 3>& rhs);
	friend std::ostream& operator<<( std::ostream& os,  vec<int, 4>& rhs);

private:
};


// names
typedef vec<float,  3> vec3f;
typedef vec<double, 3> vec3d;
typedef vec<int,    3> vec3i;

typedef vec<float,  4> vec4f;
typedef vec<double, 4> vec4d;
typedef vec<int,    4> vec4i;

typedef vec<float, 3> color3f;
typedef vec<float, 4> color4f;
typedef vec<uint16_t, 3> color3i;
typedef vec<uint16_t, 4> color4i;


vec3f operator*(float lhs, const vec3f& rhs);
vec3f operator+(float lhs, const vec3f& rhs);
vec3f operator-(float lhs, const vec3f& rhs);

vec4f operator*(float lhs, const vec4f& rhs);
vec4f operator+(float lhs, const vec4f& rhs);
vec4f operator-(float lhs, const vec4f& rhs);

vec3f operator*(float lhs, const vec3i& rhs);
vec3f operator+(float lhs, const vec3i& rhs);
vec3f operator-(float lhs, const vec3i& rhs);

color3i operator*(float lhs, const color3i& rhs);
color3i operator+(float lhs, const color3i& rhs);
color3i operator-(float lhs, const color3i& rhs);



#endif

