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

	GLuint waterTexture, skyboxTexture, dudvMap, reflectionTexture, refractionTexture, refractionDepthTexture;

	std::vector<unsigned int> indices;
	std::vector<std::vector<glm::vec3>> square_location;
	std::vector<glm::vec3> temp_vector;


	GLuint vao, vbo[3], ebo;
	GLuint reflection_fbo, refraction_fbo; //Frame buffer object
	GLuint reflection_dbo; //Depth Buffer Object

	GLuint shaderProgram;

	glm::mat4 model;

	int plane_size = 2000;
	int vertex_count = 128;

	float wave_speed = 0.00001f;
	float move_factor = 0.0f;

	float start_time;

	float reflection_width = 320.0f;
	float reflection_height = 180.0f;

	float refraction_width = 1280.0f;
	float refraction_height = 720.0f;

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
	void setCubeMap(GLuint textureID);

	void setReflectionFrameBuffer();
	void setRefractionFrameBuffer();

	void bindReflectionFrameBuffer();

	void bindRefractionFrameBuffer();

	void bindFrameBuffer(GLuint FBO, float width, float height);

	void unbindCurrentFrameBuffer(int width, int height);

	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getReflectionFBO();
	float getReflectionWidth();
	float getReflectionHeight();
};

#endif