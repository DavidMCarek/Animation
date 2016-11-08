#include "glUtils.h"


bool start_gl() {
	std::cout << "Starting GLFW" << std::endl;

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	g_window = glfwCreateWindow(
		g_gl_width, g_gl_height, "Extended Init.", NULL, NULL
		);
	if (!g_window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}
	glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);
	glfwMakeContextCurrent(g_window);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	printf("OpenGL version supported %s\n", version);
	printf("renderer: %s\nversion: %s\n", renderer, version);

	return true;
}

void glfw_error_callback(int error, const char* description) {
	std::cout << "ERROR: " << description << std::endl;
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
	g_gl_width = width;
	g_gl_height = height;
	printf("width %i height %i\n", width, height);
}

void update_fps_counter (GLFWwindow* window) {
	static double previous_seconds = glfwGetTime ();
	static int frame_count;
	double current_seconds = glfwGetTime ();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		 sprintf_s(tmp, "opengl @ fps: %.2f", fps);
		 glfwSetWindowTitle (window, tmp);
		 frame_count = 0;
	}
	frame_count++;
}