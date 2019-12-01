#ifndef _PLANE_H_
#define _PLANE_H_

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
#include <iostream>
#include <vector>
#include <memory>


#include "shader.h"
#include "Transform.h"
#include "Node.h"
#include "Geometry.h"

class Plane
{
private:
	glm::vec3 a, b, c, normal;
public:
	Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	~Plane();

	glm::vec3 getNormal();
	glm::vec3 getA();
};

#endif
