#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Plane.h"
#include "Node.h"

using namespace std;

class Geometry : public Node
{
private:
	glm::mat4 model;
	glm::mat4 initial;


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> curr_vertices;
	std::vector<glm::vec3> normals;

	std::vector<unsigned int >vertexIndices, uvIndices, normalIndices, indices;

	std::vector<glm::vec3> a_vertices;
	std::vector<glm::vec3> a_normals;

	std::vector<glm::vec3> initial_dis_points;
	std::vector<glm::vec3> scalar_dis_points;
	glm::vec3 last_mouse_pos;
	int timer;
	GLuint vao, vbo[2], ebo, lamp_program, lightvao;
	unsigned int lightVao;
	GLfloat centerx, centery, centerz;
	GLfloat new_centerx, new_centery, new_centerz;
	GLfloat pointSize;
	GLfloat maxDistance;

	GLuint mat_select;
	glm::mat4 view, projection;
	GLFWwindow* this_window;
	int width, height;
	bool Illumination_model;
	glm::vec3 rotAxis;
	GLfloat angle;

	static bool Phong_shading;
	bool render;
	bool render_in_lines;
	bool render_in_triangle;

	glm::vec3 normal;
	glm::vec3 color;
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
	GLfloat attenuation;
	glm::vec3 viewPos;
	glm::vec3 center;

public:
	Geometry(std::string objFilename, GLuint mat_select);
	~Geometry();

	void update(glm::mat4 C);
	void draw(glm::mat4 c, GLuint program);
	void scale(glm::vec3 scale_value);
	void alignData();
	void set_Materials();
	void toggle_render();
	void setUniformVariable(GLuint program);
	void set_render_method(GLuint method);
	void set_render(bool render);
	glm::mat4 get_model();
	bool getRender();
	GLfloat getMaxDistance();
};


#endif

