#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal_vec;

uniform mat4 persMat;
uniform vec3 in_color;
uniform mat4 model_mat;

out vec3 norm_vec;
out vec3 color;

void main () {

	norm_vec = normal_vec;
	color = in_color;
	gl_Position = persMat * model_mat * vec4 (vp, 1.0);

}