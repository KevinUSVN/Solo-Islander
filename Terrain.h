#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Node.h"
#include <math.h>
#include <ctime>
#include "Window.h"

class Terrain
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<std::string> list_textures;
	GLuint terrainTexture;

	std::vector<unsigned int> indices;
	std::vector<std::vector<glm::vec3>> square_location;
	std::vector<glm::vec3> temp_vector;

	float max_height;
	float min_height;

	GLuint vao, vbo[3], ebo;
	GLuint shaderProgram;

	glm::mat4 model;

	float max_displacement = 0.01f;
	int terrain_size = 500;
	int vertex_count = 128;

	float range = 4.0f;
	int level = (int)glm::pow(2, 16.0f);

public:
	Terrain(GLuint program);
	~Terrain();

	void initTerrain();

	void diamondSquare(int x1, int x2, int y1, int y2, int level, float range);
	
	void setBuffers();
	void draw();
	void update();
	void setupTextures();
	GLuint loadTextures(std::vector<std::string> list_textures);
};

#endif