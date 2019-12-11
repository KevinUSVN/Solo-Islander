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
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Plane.h"
#include "Node.h"

using namespace std;

enum BOUNDING_BOX { TLN = 0, TRN = 1, BLN = 2, BRN = 3, TLF = 4, TRF = 5, BLF = 6, BRF = 7 };

class Geometry : public Node
{
private:
	glm::mat4 model;
	glm::mat4 initial;


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> curr_vertices;
	std::vector<glm::vec3> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned int >vertexIndices, uvIndices, normalIndices, indices;

	std::vector<glm::vec3> a_vertices;
	std::vector<glm::vec3> a_normals;

	std::vector<glm::vec3> initial_dis_points;
	std::vector<glm::vec3> scalar_dis_points;
	glm::vec3 last_mouse_pos;
	int timer;
	GLuint vao, vbo[2], ebo[2], lamp_program, lightvao;
	unsigned int lightVao;
	GLfloat centerx, centery, centerz;
	GLfloat new_centerx, new_centery, new_centerz;
	GLfloat pointSize;
	GLfloat maxDistance;
	GLfloat max_x, min_x, max_y, min_y, max_z, min_z;
	std::vector<glm::vec3> bounding_variables[8];

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

	//Procedural object.
	std::vector<Geometry*> next_object;
	//Write parser for txt file.
	//Write parser for procedurally generated shapes.
	//Write file for arbitary shapes.

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
	GLfloat getMax_x();
	GLfloat getMax_y();
	GLfloat getMax_z();
	GLfloat getMin_x();
	GLfloat getMin_y();
	GLfloat getMin_z();


};


#endif

