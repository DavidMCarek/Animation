#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 normal_vec;
layout(location = 2) in vec2 texture_coord;

uniform mat4 persMat;
uniform mat4 model_mat;

out vec3 norm_vec;
out vec2 tc_out;

void main () {
	
	tc_out = texture_coord;
	norm_vec = normal_vec;
	gl_Position = persMat * model_mat * vec4 (vp, 1.0);

}