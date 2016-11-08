#include "mat4.h"

mat4::mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
{
	mat[0] = a;
	mat[1] = b;
	mat[2] = c;
	mat[3] = d;
	mat[4] = e;
	mat[5] = f;
	mat[6] = g;
	mat[7] = h;
	mat[8] = i;
	mat[9] = j;
	mat[10] = k;
	mat[11] = l;
	mat[12] = m;
	mat[13] = n;
	mat[14] = o;
	mat[15] = p;
}

mat4::~mat4()
{
}

mat4 operator*(const mat4 & u, const mat4 & v)
{
	const float *a = u.mat;
	const float *b = v.mat;

	float c0 = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	float c1 = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	float c2 = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	float c3 = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

	float c4 = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	float c5 = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	float c6 = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	float c7 = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

	float c8 = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	float c9 = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	float c10 = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	float c11 = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

	float c12 = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	float c13 = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	float c14 = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	float c15 = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];

	return mat4(
		c0, c1, c2, c3,
		c4, c5, c6, c7,
		c8, c9, c10, c11,
		c12, c13, c14, c15
		);
}

mat4 make_cam_mat(vec3 camPos, vec3 camTar, vec3 upVec) {
	vec3 u = (camTar - camPos).normalize();
	vec3 v = u.cross(upVec).normalize();
	vec3 w = v.cross(u);

	mat4 m = mat4(
		1, 0, 0, -camPos.x,
		0, 1, 0, -camPos.y,
		0, 0, 1, -camPos.z,
		0, 0, 0, 1
		);

	mat4 n = mat4(
		v.x, v.y, v.z, 0,
		w.x, w.y, w.z, 0,
		-u.x, -u.y, -u.z, 0,
		0, 0, 0, 1
		);

	return n * m;
}

mat4 rotate_y_mat(float rotationAngle) {
	return mat4(
		cos(rotationAngle),	0.0, sin(rotationAngle), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rotationAngle), 0.0, cos(rotationAngle), 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

mat4 get_translation_mat(vec3 translation) {
	return mat4(
		1.0, 0.0, 0.0, translation.x,
		0.0, 1.0, 0.0, translation.y,
		0.0, 0.0, 1.0, translation.z,
		0.0, 0.0, 0.0, 1.0
		);
}

mat4 perspective_projection(float fov, float aspect, float near, float far) {
	float f = 1.0 / tan(fov * DEGREES_PER_RADIAN / 2.0);
	float denominator = near - far;

	float a = f / aspect;
	float b = f;
	float c = (far + near) / denominator;
	float d = (2.0 * far * near) / denominator;

	return mat4(
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, d,
		0, 0, -1, 0
		);
}

mat4 scale_mat(float scaleFactor) {
	return mat4(
		scaleFactor, 0.0, 0.0, 0.0,
		0.0, scaleFactor, 0.0, 0.0,
		0.0, 0.0, scaleFactor, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}

mat4 identity_mat()
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
		);
}
