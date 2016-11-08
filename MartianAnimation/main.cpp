#include <stdio.h>
#include <vector>
#include <algorithm>
#include "glUtils.h"
#include "loadShader.h"
#include "vec3.h"
#include "mat4.h"
#include "loadObj.h"

int g_gl_width = 960;
int g_gl_height = 540;

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

	const char* vertex_shader = load_shader(".\\vert.glsl");
	const char* fragment_shader = load_shader(".\\frag.glsl");

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	check_shader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	check_shader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);
	glBindVertexArray(plane_vao);

	vec3 cam_location = vec3(-1.0, 1.0, 0.0);
	vec3 cam_target = vec3(1.0, M_PI_4 + M_PI_2, 0.0);
	vec3 up = vec3(0, 1, 0);

	mat4 camMat = make_cam_mat(cam_location, to_cartesian(cam_target) + cam_location, up);

	mat4 persMat4 = perspective_projection(67, (float)g_gl_width / g_gl_height, 0.1, 100);

	GLuint persMat = glGetUniformLocation(shader_programme, "persMat");

	GLuint color = glGetUniformLocation(shader_programme, "in_color");

	GLuint model_mat = glGetUniformLocation(shader_programme, "model_mat");

	std::vector<vec3> p_martian;
	std::vector<vec3> n_martian;
	std::vector<vec2> t_martian;
	load_obj_file(martian_helmet, p_martian, n_martian, t_martian);
	load_obj_file(martian_body, p_martian, n_martian, t_martian);
	load_obj_file(martian_shoes, p_martian, n_martian, t_martian);

	GLuint entity_vao = 0;
	glGenVertexArrays(1, &entity_vao);
	glBindVertexArray(entity_vao);

	GLuint entity_vbo = 0;
	buffer_setup(entity_vbo, p_martian.data(), p_martian.size() * sizeof(vec3), 0);

	GLuint entity_n_vbo = 0;
	buffer_setup(entity_vbo, n_martian.data(), n_martian.size() * sizeof(vec3), 1);

	float angle = 0.0;

	float x = 0.0;
	float y = base_terrain_height;
	float z = 0.0;

	vec3 martian_position(0, 0, 0.5);
	float martian_rotation = 0.0;
	vec3 martian_color(0.0, 1.0, 0.0);

	mat4 translation_mat1 = get_translation_mat(vec3(x, y, z));

	int loop_count = 0;

	while (!glfwWindowShouldClose(g_window)) {
		static double previous_seconds = glfwGetTime();
		double current_seconds = glfwGetTime();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		update_fps_counter(g_window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shader_programme);

		glUniformMatrix4fv(persMat, 1, GL_TRUE, (persMat4 * camMat).mat);

		glUniformMatrix4fv(model_mat, 1, GL_TRUE, identity_mat().mat);

		glUniform3f(color, 0.75, 0.75, 0.75);
		glBindVertexArray(plane_vao);
		glDrawArrays(GL_TRIANGLES, 0, pointCount);

		martian_position.x = bez_x(loop_count);
		martian_position.z = bez_z(loop_count);


		glUniform3f(color, martian_color.x, martian_color.y, martian_color.z);
		glUniformMatrix4fv(model_mat, 1, GL_TRUE, (get_translation_mat(martian_position) * rotate_y_mat(martian_rotation) * scale_mat(0.05)).mat);
		glBindVertexArray(entity_vao);
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

		loop_count++;

		loop_count %= granularity;

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