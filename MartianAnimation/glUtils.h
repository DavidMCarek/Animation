#pragma once

#include <iostream>
#include "GL\glew.h"
#include "GLFW\glfw3.h"

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;
bool start_gl();

void glfw_error_callback(int error, const char* description);

void glfw_window_size_callback(GLFWwindow* window, int width, int height);

void update_fps_counter(GLFWwindow* window);