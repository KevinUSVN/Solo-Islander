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
	if (position.y <= 0.0f) {
		this->position = glm::vec3(position.x, 0.0f, position.y);
	}
	else {
		this->position = position;
	}
}