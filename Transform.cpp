#include "Transform.h"

void Transform::addChild(Node* child)
{
	this->children.push_back(child);
}

bool Transform::get_render()
{
	return render;
}
void Transform::draw(glm::mat4 c, GLuint program)
{
	if (render == true) {
		for (auto child : children)
		{
			child->draw(c * TransformM, program);
		}
	}
}

Transform::Transform(glm::mat4 i)
{
	this->TransformM = i;
	render = true;
}

Transform::~Transform()
{
}

void Transform::update(glm::mat4 C)
{
	TransformM = TransformM * C;
	for (auto child : children)
	{
		child->update(C);
	}
}

void Transform::set_render(bool ren)
{
	render = ren;
}

glm::mat4 Transform::getTransform()
{
	return TransformM;
}