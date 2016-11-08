#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "vec3.h"
#include "vec2.h"
#include <vector>

void load_obj_file(std::string file, std::vector<vec3> &vertices, std::vector<vec3> &normals, std::vector<vec2> &textures);