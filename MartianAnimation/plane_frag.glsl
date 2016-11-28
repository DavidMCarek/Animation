in vec3 norm_vec;
in vec3 color;
uniform mat4 model_mat;

out vec4 frag_colour;

void main () {

	vec3 light1 = normalize(vec3 (1, 0, 1));
	vec3 light2 = normalize(vec3 (1, 1, 1));
	vec3 light3 = normalize(vec3 (-1, 1, 1));
	vec3 ambient = normalize(vec3 (0, 0, 0));
	mat3 it = inverse(transpose(mat3(model_mat)));
	vec3 norm_n = normalize(it * norm_vec);
	float eff_light = (max(dot(light1, norm_n), 0.0)) + 0.1;
	frag_colour = vec4 (color * eff_light, 1.0);
	frag_colour = pow(frag_colour, vec4(1/2.2, 1/2.2, 1/2.2, 1));
}