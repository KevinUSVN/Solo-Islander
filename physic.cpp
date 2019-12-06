#include "physic.h"

Physic::Physic()
{
	this->time_before_a_stop = 0.0f;
	this->acceleration = glm::vec3(0.0f,0.0f,0.0f);
	this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}
Physic::Physic(GLfloat time_before_a_stop, glm::vec3 acceleration, glm::vec3 init_Vel, glm::vec3 init_Pos)
{
	this->time_before_a_stop = time_before_a_stop;
	this->acceleration = acceleration;
	this->velocity = init_Vel;
	this->position = init_Pos;
}

Physic::~Physic()
{
}

glm::vec3 Physic::calculate_new_Pos(GLfloat new_time)
{
	GLfloat timediff = new_time - this->time_before_a_stop;
	setVelocity(this->velocity + (glm::vec3((this->acceleration.x*timediff), (this->acceleration.y*timediff), (this->acceleration.z*timediff))));
	setPosition(this->position + (glm::vec3(this->velocity.x * timediff, this->velocity.y * timediff, this->velocity.z * timediff)));
	return this->position;
}

void Physic::setAcceleration(glm::vec3 acceleration)
{
	this->acceleration = acceleration;
}

void Physic::setVelocity(glm::vec3 velocity)
{
	this->velocity = velocity;
}

void Physic::setPosition(glm::vec3 position)
{
	if (position.y <= 10.0f) {
		this->position = glm::vec3(position.x, 10.0f, position.y);
	}
	else {
		this->position = position;
	}
}

GLfloat Physic::detect_terrain_height(std::vector<std::vector<glm::vec3>> terrainXZ, glm::vec3 Cam_Pos)
{
	GLfloat x = Cam_Pos.x;
	GLfloat z = Cam_Pos.z;
	
	GLint x_to_map = x / (1000.0f / 128.0f);
	GLint z_to_map = z / (1000.0f / 128.0f);
	GLint vertices_Location = (128 * z_to_map) + x_to_map;
	std::cout << vertices_Location << std::endl;

	return 1;
}
