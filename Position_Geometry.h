#ifndef _POSITION_GEOMETRY_H
#define _POSITION_GEOMETRY_H

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

class Position_Geometry
{
	private:
		vector<pair<glm::vec3 , Geometry*>> geometry_on_position;
	public:
		Position_Geometry();
		~Position_Geometry();

		void push_Geometries(Geometry* geometry, glm::vec3 xyz);
		vector<pair<glm::vec3, Geometry*>> get_Geometries();
		bool is_Empty();
};


#endif