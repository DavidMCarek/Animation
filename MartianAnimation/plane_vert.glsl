#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal_vec;

uniform mat4 persMat;
uniform mat4 model_mat;
uniform vec3 in_color;

out vec3 norm_vec;
out vec3 color;

void main () {
	
	color = in_color;
	norm_vec = normal_vec;
	gl_Position = persMat * model_mat * vec4 (vp, 1.0);

}