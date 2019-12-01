#ifndef _NODE_H
#define _NODE_H

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
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include<time.h> 
#include <glm/gtc/type_ptr.hpp>
class Node
{
private:

public:
	Node();
	~Node();
	virtual void draw(glm::mat4 c, GLuint shader_program) = 0;
	virtual void update(glm::mat4 c) = 0;
};


#endif


