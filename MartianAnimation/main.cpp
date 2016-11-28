#include <stdio.h>
#include <vector>
#include <algorithm>
#include "glUtils.h"
#include "loadShader.h"
#include "vec3.h"
#include "mat4.h"
#include "loadObj.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int g_gl_width = 960;
int g_gl_height = 540;

std::string rex_obj = "allo.obj";
std::string rex_tex = "allo.png";

std::string marvin_martian = "marvinDos2ndTry.obj";
std::string martian_shoes = "martianShoes.obj";
std::string martian_helmet = "martianHelmet.obj";
std::string martian_body = "martianBodyHeadHands.obj";

float cam_sensitivity = 0.005f;
float cam_speed = 0.025f;
float entity_sensitivity = 0.025f;
float entity_speed = 0.015f;
float upper_map_bound = 0.6f;
float lower_map_bound = -0.6f;
float max_height = 1.0f;
float min_height = 0.0f;
float base_terrain_height = 0.0f;
int granularity = 500;

GLFWwindow* g_window = NULL;

float bounds(float pos);
void buffer_setup(GLuint vbo, float * points, int size, int attrib_array);
void buffer_setup(GLuint vbo, vec3 * points, int size, int attrib_array);
void buffer_setup(GLuint vbo, vec2 * points, int size, int attrib_array);
float bez_x(int count);
float bez_z(int count);

int main() {
	
	start_gl();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	float plane[] = {
		lower_map_bound, base_terrain_height, lower_map_bound,
		upper_map_bound, base_terrain_height, lower_map_bound,
		upper_map_bound, base_terrain_height, upper_map_bound,
		lower_map_bound, base_terrain_height, lower_map_bound,
		upper_map_bound, base_terrain_height, upper_map_bound,
		lower_map_bound, base_terrain_height, upper_map_bound
	};

	float plane_n[] = {
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};

	int pointCount = 6;

	GLuint plane_vao = 0;
	glGenVertexArrays(1, &plane_vao);
	glBindVertexArray(plane_vao);
	
	GLuint plane_vbo = 0;
	buffer_setup(plane_vbo, plane, 18 * sizeof(float), 0);

	GLuint plane_n_vbo = 0;
	buffer_setup(plane_n_vbo, plane_n, 18 * sizeof(float), 1);
	glBindVertexArray(plane_vao);

	GLuint plane_shader_program = create_shader_program(load_shader(".\\plane_frag.glsl"), load_shader(".\\plane_vert.glsl"));

	GLuint entity_shader_program = create_shader_program(load_shader(".\\frag.glsl"), load_shader(".\\vert.glsl"));

	vec3 cam_location = vec3(-1.0, 1.0, 0.0);
	vec3 cam_target = vec3(1.0, M_PI_4 + M_PI_2, 0.0);
	vec3 up = vec3(0, 1, 0);

	mat4 camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);

	mat4 persMat4 = perspective_projection(67, (float)g_gl_width / g_gl_height, 0.1, 100);

	GLuint plane_persMat = glGetUniformLocation(plane_shader_program, "persMat");

	GLuint color = glGetUniformLocation(plane_shader_program, "in_color");

	GLuint plane_model_mat = glGetUniformLocation(plane_shader_program, "model_mat");

	GLuint persMat = glGetUniformLocation(entity_shader_program, "persMat");

	GLuint model_mat = glGetUniformLocation(entity_shader_program, "model_mat");

	GLuint light_0 = glGetUniformLocation(entity_shader_program, "light_0");
	GLuint light_1 = glGetUniformLocation(entity_shader_program, "light_1");
	GLuint light_2 = glGetUniformLocation(entity_shader_program, "light_2");

	std::vector<vec3> p_martian;
	std::vector<vec3> n_martian;
	std::vector<vec2> t_martian;
	load_obj_file(rex_obj, p_martian, n_martian, t_martian);

	stbi_set_flip_vertically_on_load(1);
	int tex_x, tex_y, tex_n;
	tex_n = 3;
	unsigned char *data1 = stbi_load(rex_tex.c_str(), &tex_x, &tex_y, &tex_n, 3);
	
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_x, tex_y, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	stbi_image_free(data1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint entity_vao = 0;
	glGenVertexArrays(1, &entity_vao);
	glBindVertexArray(entity_vao);

	GLuint entity_vbo = 0;
	buffer_setup(entity_vbo, p_martian.data(), p_martian.size() * sizeof(vec3), 0);

	GLuint entity_n_vbo = 0;
	buffer_setup(entity_vbo, n_martian.data(), n_martian.size() * sizeof(vec3), 1);

	GLuint entity_t_vbo = 0;
	buffer_setup(entity_vbo, t_martian.data(), t_martian.size() * sizeof(vec2), 2);

	float angle = 0.0;

	float x = 0.0;
	float y = base_terrain_height;
	float z = 0.0;

	vec3 entity_position(0, 0, 0.5);
	float entity_rotation_x = 3 * M_PI_2;
	float entity_rotation_y = 3 * M_PI_2;
	float entity_rotation_z = 0.0;

	mat4 translation_mat1 = get_translation_mat(vec3(x, y, z));

	int loop_count = 0;

	bool pause = false;

	GLboolean l0 = true;
	GLboolean l1 = true;
	GLboolean l2 = true;

	bool keys_down[GLFW_KEY_LAST];

	while (!glfwWindowShouldClose(g_window)) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		update_fps_counter(g_window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(plane_shader_program);

		glUniformMatrix4fv(plane_persMat, 1, GL_TRUE, (persMat4 * camMat).mat);

		glUniformMatrix4fv(plane_model_mat, 1, GL_TRUE, identity_mat().mat);

		glUniform3f(color, 0.75, 0.75, 0.75);
		glBindVertexArray(plane_vao);
		glDrawArrays(GL_TRIANGLES, 0, pointCount);

		glUseProgram(entity_shader_program);

		glUniform1i(light_0, l0);
		glUniform1i(light_0, l1);
		glUniform1i(light_0, l2);

		glUniformMatrix4fv(persMat, 1, GL_TRUE, (persMat4 * camMat).mat);

		glUniformMatrix4fv(model_mat, 1, GL_TRUE, identity_mat().mat);

		if (!pause) {
			entity_position.x = bez_x(loop_count);
			entity_position.z = bez_z(loop_count);
		}

		glUniformMatrix4fv(model_mat, 1, GL_TRUE, 
			(get_translation_mat(entity_position) *
			rotate_z_mat(entity_rotation_z) *
			rotate_y_mat(entity_rotation_y) *
			rotate_x_mat(entity_rotation_x) *
			scale_mat(0.05)).mat);
		glBindVertexArray(entity_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawArrays(GL_TRIANGLES, 0, p_martian.size());

		glfwPollEvents();

		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(g_window, 1);
		}

		if (glfwGetKey(g_window, GLFW_KEY_S)) {
			vec3 movement_vec = vec3(cam_speed, M_PI_2, cam_target.z);
			cam_location = cam_location - to_cartesian(movement_vec);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_W)) {
			vec3 movement_vec = vec3(cam_speed, M_PI_2, cam_target.z);
			cam_location = cam_location + to_cartesian(movement_vec);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_D)) {
			vec3 movement_vec = vec3(cam_speed, M_PI_2, cam_target.z + M_PI_2);
			cam_location = cam_location + to_cartesian(movement_vec);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_A)) {
			vec3 movement_vec = vec3(cam_speed, M_PI_2, cam_target.z - M_PI_2);
			cam_location = cam_location + to_cartesian(movement_vec);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_SPACE)) {
			cam_location.y += cam_speed;
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_LEFT_SHIFT)) {
			cam_location.y -= cam_speed;
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_LEFT)) {
			cam_target.z -= M_PI * cam_sensitivity;
			if (cam_target.z < 0.0)
				cam_target.z = 2 * M_PI;
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_RIGHT)) {
			cam_target.z += M_PI * cam_sensitivity;
			if (cam_target.z >= 2 * M_PI)
				cam_target.z = 0.0;
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_DOWN)) {
			cam_target.y += M_PI * cam_sensitivity;
			if (cam_target.y >= M_PI - (M_PI / 64))
				cam_target.y = M_PI - (M_PI / 64);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_UP)) {
			cam_target.y -= M_PI * cam_sensitivity;
			if (cam_target.y < (M_PI / 64))
				cam_target.y = (M_PI / 64);
			camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);
		}
		if (glfwGetKey(g_window, GLFW_KEY_0)) {
			keys_down[GLFW_KEY_0] = true;
		}
		else {
			if (keys_down[GLFW_KEY_0]) {
				l0 = !l0;
				keys_down[GLFW_KEY_0] = false;
			}
		}
		if (glfwGetKey(g_window, GLFW_KEY_1)) {
			keys_down[GLFW_KEY_1] = true;
		}
		else {
			if (keys_down[GLFW_KEY_1]) {
				l1 = !l1;
				keys_down[GLFW_KEY_1] = false;
			}
		}
		if (glfwGetKey(g_window, GLFW_KEY_2)) {
			keys_down[GLFW_KEY_2] = true;
		}
		else {
			if (keys_down[GLFW_KEY_2]) {
				l2 = !l2;
				keys_down[GLFW_KEY_2] = false;
			}
		}
		if (glfwGetKey(g_window, GLFW_KEY_P)) {
			keys_down[GLFW_KEY_P] = true;
		}
		else {
			if (keys_down[GLFW_KEY_P]) {
				pause = !pause;
				keys_down[GLFW_KEY_P] = false;
			}
		}

		if (glfwGetKey(g_window, GLFW_KEY_Q)) {
			return 0;
		}

		if (!pause) {
			loop_count++;
			loop_count %= granularity;

			entity_rotation_y -= ((2.0 * M_PI) / granularity);
			entity_rotation_y = entity_rotation_y < 0.0 ? (2.0 * M_PI) : entity_rotation_y;
		}

		glfwSwapBuffers(g_window);
	}


	glfwTerminate();
	return 0;
}

float bez_x(int count) {
	float t = (float)count / granularity;
	float result = -9.0 * t * t * t;
	result += 13.5 * t * t;
	result -= 4.5 * t;
	return result;
}

float bez_z(int count) {
	float t = (float)count / granularity;
	float result = 1.5 * t * t * t;
	result -= 1.5 * t;
	result += 0.5;
	return result;
}

float bounds(float pos) {
	if (pos > upper_map_bound)
		return upper_map_bound;
	if (pos < lower_map_bound)
		return lower_map_bound;
	return pos;
}


// make a template later
void buffer_setup(GLuint vbo, float * points, int size, int attrib_array) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_array, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attrib_array);
}

void buffer_setup(GLuint vbo, vec3 * points, int size, int attrib_array) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_array, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attrib_array);
}

void buffer_setup(GLuint vbo, vec2 * points, int size, int attrib_array) {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
	glVertexAttribPointer(attrib_array, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attrib_array);
}