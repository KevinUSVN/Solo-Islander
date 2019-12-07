#ifndef _WATER_H_
#define _WATER_H_

#include "Node.h"
#include <math.h>
#include <ctime>
#include "Window.h"

class Water
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<std::string> list_textures;
	GLuint waterTexture1;

	std::vector<unsigned int> indices;
	std::vector<std::vector<glm::vec3>> square_location;
	std::vector<glm::vec3> temp_vector;


	GLuint vao, vbo[3], ebo;
	GLuint shaderProgram;

	glm::mat4 model;

	int plane_size = 2000;
	int vertex_count = 128;

public:
	Water(GLuint program);
	~Water();

	void initWater();
	void setBuffers();
	
	void draw();
	void update();
	std::vector<std::vector<glm::vec3>> getVertex();

	void setupTextures();
	GLuint loadTexture(std::string filePath);
};

#endif