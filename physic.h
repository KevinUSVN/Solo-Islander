#ifndef _PHYSIC_H
#define _PHYSIC_H

#ifdef __APPLE__
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
#include <vector>
#include <memory>
#include <GL/glut.h>


class Physic
{
private:
	GLfloat time_before_a_stop;
	GLfloat new_time;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::vec3 position;
	bool collision_detected;
public:
	Physic();
	Physic(GLfloat time_before_a_stop, glm::vec3 acceleration, glm::vec3 init_Vel, glm::vec3 init_Pos);
	~Physic();
	
	glm::vec3 calculate_new_Pos(GLfloat new_time);
	void setAcceleration(glm::vec3 acceleration);
	void setVelocity(glm::vec3 veolocity);
	void setPosition(glm::vec3 position);

	GLfloat detect_terrain_height(std::vector<std::vector<glm::vec3>> terrainXZ, glm::vec3 Cam_Pos);
};


#endif
