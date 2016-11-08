#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

class vec3
{
public:
	vec3(float x, float y, float z);
	~vec3();
	float x;
	float y;
	float z;

	vec3 cross(const vec3 &v);
	vec3 normalize();
	float length();
};

vec3 operator-(const vec3 &u, const vec3 &v);
vec3 operator+(const vec3 &u, const vec3 &v);

vec3 to_sphereical(vec3 vec);
vec3 to_cartesian(vec3 vec);