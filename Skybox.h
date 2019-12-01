#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include "shader.h"



class Skybox
{
private:
	std::vector<GLfloat> cubeVertices;
	std::vector<GLfloat> skyboxVertices;
	std::vector<std::string> faces;
	glm::mat4 model;
	GLuint Shader_program;
	GLuint skyboxID;
	GLuint cubeVAO, cubeVBO;
	GLuint skyboxVAO, skyboxVBO;
	GLuint cubemapTexture, cubeTexture;
public:
	Skybox();
	~Skybox();

	void set_Cube_Shader(GLuint program);
	unsigned int loadTexture(std::string path);
	unsigned int loadCubemap(std::vector<std::string> faces);
	void drawSkybox();

};







#endif
