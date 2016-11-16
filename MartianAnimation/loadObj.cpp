#include "loadObj.h"
#include "strUtils.h"
#include <stdio.h>

struct Face {
	int v1;
	int v2;
	int v3;
};

void load_obj_file(std::string file, std::vector<vec3> &vertices, std::vector<vec3> &normals, std::vector<vec2> &textures) {

	std::ifstream obj;

	obj.open(file, std::ios::binary);

	if (obj.fail()) {
		std::cout << "failed to open " << file << std::endl;
	}
	
	std::vector<Face> vertex_indices, uv_indices, normal_indices;
	std::vector<vec3> temp_vertices;
	std::vector<vec2> temp_uvs;
	std::vector<vec3> temp_normals;

	std::string line_header;

	char str_line[128];

	int count = 0;
	char c = 0;

	while (c != EOF) {
		c = obj.get();

		if (c != '\n' && c != EOF) {
			str_line[count] = c;
		}
		else {
			if (count == 0 && c == EOF)
				break;

			str_line[count] = '\0';

			std::string line(str_line);

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ' ') {
					line_header = line.substr(0, i);
					line = line.substr(i + 1, line.length() - i - 1);
					break;
				}
			}

			const char *str = line.c_str();

			if (line_header == "v") {
				vec3 vertex(0, 0, 0);
				sscanf_s(str, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (line_header == "vt") {
				vec2 uv(0, 0);
				sscanf_s(str, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if (line_header == "vn") {
				vec3 normal(0, 0, 0);
				sscanf_s(str, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (line_header == "f") {
				unsigned int vertex_index[3], uv_index[3], normal_index[3];
				int matches = sscanf_s(str, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);
				if (matches != 9) {
					printf("faces can't be read by parser\n");
					return;
				}

				Face vertex;
				vertex.v1 = vertex_index[0] - 1;
				vertex.v2 = vertex_index[1] - 1;
				vertex.v3 = vertex_index[2] - 1;
				vertex_indices.push_back(vertex);

				Face normal;
				normal.v1 = normal_index[0] - 1;
				normal.v2 = normal_index[1] - 1;
				normal.v3 = normal_index[2] - 1;
				normal_indices.push_back(normal);

				Face texture;
				texture.v1 = uv_index[0] - 1;
				texture.v2 = uv_index[1] - 1;
				texture.v3 = uv_index[2] - 1;
				uv_indices.push_back(texture);
			}

			count = 0;
			continue;
		}

		count++;
	}

	for (int i = 0; i < vertex_indices.size(); i++) {
		vertices.push_back(temp_vertices[vertex_indices[i].v1]);
		vertices.push_back(temp_vertices[vertex_indices[i].v2]);
		vertices.push_back(temp_vertices[vertex_indices[i].v3]);
	}
	for (int i = 0; i < uv_indices.size(); i++) {
		textures.push_back(temp_uvs[uv_indices[i].v1]);
		textures.push_back(temp_uvs[uv_indices[i].v2]);
		textures.push_back(temp_uvs[uv_indices[i].v3]);
	}
	for (int i = 0; i < normal_indices.size(); i++) {
		normals.push_back(temp_normals[normal_indices[i].v1]);
		normals.push_back(temp_normals[normal_indices[i].v2]);
		normals.push_back(temp_normals[normal_indices[i].v3]);
	}

}