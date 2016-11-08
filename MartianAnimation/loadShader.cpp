#include "loadShader.h"

char* load_shader(std::string file) {
	std::ifstream shader;

	shader.open(file, std::ios::binary);

	if (shader.fail()) {
		std::cout << "failed to open " << file << std::endl;
		return "";
	}

	shader.seekg(0, shader.end);
	int length = shader.tellg();
	shader.seekg(0, shader.beg);

	char * fileContents = new char[length + 1];
	fileContents[length] = 0;

	shader.read(fileContents, length);

	shader.close();

	return fileContents;
}

void check_shader(GLuint shader) {
	int params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (params != GL_TRUE) {
		char *log = new char[2048];
		int length;
		glGetShaderInfoLog(shader, 2048, &length, log);
		printf("%s\n", log);
	}
}