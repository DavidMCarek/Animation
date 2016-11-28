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

