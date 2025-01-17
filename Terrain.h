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

	GLuint terrainTexture1, terrainTexture2, terrainTexture3, terrainTexture4;

	std::vector<unsigned int> indices;
	std::vector<std::vector<glm::vec3>> square_location;
	std::vector<glm::vec3> temp_vector;

	GLuint vao, vbo[3], ebo;
	GLuint shaderProgram;

	glm::mat4 model;

	float max_displacement = 0.01f;
	GLfloat terrain_size = 1024;
	GLfloat vertex_count = 128;

	float range = 4.0f;
	int level = (int)glm::pow(2, 16.0f);

public:
	Terrain(GLuint program);
	~Terrain();

	void initTerrain();
	void setBuffers();

	void diamondSquare(int x1, int x2, int y1, int y2, int level, float range);
	
	void draw();
	void fixEdges();
	void update();

	std::vector<std::vector<glm::vec3>> getVertex();
	float getHeightFromVertex(int x, int y);
	GLfloat getTerrainSize();
	GLfloat getVertexCount();
	unsigned int getSquareAmount();
	void updateNormals();

	GLuint loadTexture(std::string filePath);
	void setupTextures();

	GLfloat find_terrain_height(glm::vec3 Cam_Pos);
	GLfloat barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 Pos, GLfloat scale_diff);

};

#endif