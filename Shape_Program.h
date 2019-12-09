#ifndef _SHAPE_PROGRAM_H
#define _SHAPE_PROGRAM_H

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <tuple>
#include "shader.h"
#include "Transform.h"
#include "Node.h"
#include "Geometry.h"
#include "Node.h"
#include "physic.h"
#include "Plane.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Water.h"
#include <GL/glut.h>

class Shape_Program
{
	private:
		glm::mat4 model;
		glm::mat4 initial;
		std::vector< pair<std::string, std::string>> index_OBJ;
		std::vector<Geometry*> allGeometry;
		std::vector<std::tuple<Transform*,std::string, vector<std::string>>> allTransform;
		std::string starting_object;
		Transform* head_transform;
	public:
		Shape_Program(std::string file_name, Transform * transform);
		~Shape_Program();

		void connect_Obj(std::string starting_obj, Transform * head_transform);
		void draw(glm::mat4 matrix, GLuint program);
};

#endif