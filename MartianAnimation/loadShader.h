#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "GL\glew.h"

char *load_shader(std::string file);
void check_shader(GLuint shader);

