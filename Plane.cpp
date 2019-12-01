#include "Plane.h"

Plane::Plane(glm::vec3 d, glm::vec3 e, glm::vec3 f)
{
	this->a = d;
	this->b = e;
	this->c = f;

	this->normal = glm::normalize(glm::cross((a - b), (c - b)));

}

Plane::~Plane()
{
}

glm::vec3 Plane::getNormal()
{
	return this->normal;
}

glm::vec3 Plane::getA()
{
	return this->a;
}