#version 400

out vec4 frag_colour;

in vec3 norm_vec;
in vec2 tc_out;
in vec3 fp;

uniform mat4 model_mat;
uniform bool light_0;
uniform bool light_1;
uniform bool light_2;
uniform sampler2D texture;

void main () {

	vec3 intensity = vec3(0.95, 0.95, 0.95);

	float le0, le1, le2;
	le0 = le1 = le2 = 0.0;

	if (light_0)
		le0 = 0.333;
	if (light_1)
		le1 = 0.333;
	if (light_2)
		le2 = 0.333;

	vec4 tex_color = texture(texture, tc_out);
	vec3 light0 = normalize(vec3 (1, 1, 0));
	vec3 light1 = normalize(vec3 (1, 1, 1));
	vec3 light2 = normalize(vec3 (1, 1, -1));

	mat3 normal_mat = inverse(transpose(mat3(model_mat)));
	vec3 norm_n = normalize(normal_mat * norm_vec);
	vec3 frag_pos = vec3(model_mat * vec4(fp, 1));
	vec3 stl1 = light1 - frag_pos;
	vec3 stl2 = light2 - frag_pos;

	float brightness = ((dot(norm_n, stl1) * le1) + (dot(norm_n, stl2) * le2)) / (length(stl1) * length(stl2) * 0.25 * length(norm_n));
	brightness = brightness + (max(dot(light0, norm_n), 0.0) * le0);
    brightness = clamp(brightness, 0.05, 1);

	vec4 surface_color = texture(texture, tc_out);

	frag_colour = vec4(brightness * intensity * surface_color.rgb, 1);
	frag_colour = pow(frag_colour, vec4(1/2.2, 1/2.2, 1/2.2, 1));
}