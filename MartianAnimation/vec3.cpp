#include "vec3.h"
#include <math.h>

vec3::vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3::~vec3() {}

vec3 vec3::cross(const vec3 & v) {
	return vec3(
		(this->y * v.z) - (this->z * v.y),
		(this->z * v.x) - (this->x * v.z),
		(this->x * v.y) - (this->y * v.x));
}

vec3 vec3::normalize() {
	return vec3(this->x/this->length(), this->y/this->length(), this->z/this->length());
}

float vec3::length() {
	return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 to_sphereical(vec3 vec) {
	return vec3(
		(vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z),
		acosf(vec.z / ((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z))),
		atanf(vec.y / vec.x)
		);
}

vec3 to_cartesian(vec3 vec) {
	return vec3(
		vec.x * sinf(vec.y) * cosf(vec.z),
		vec.x * cosf(vec.y),
		vec.x * sinf(vec.y) * sinf(vec.z)
		);
}