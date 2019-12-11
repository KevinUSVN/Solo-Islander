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
	if (position.y <= terrain_height) {
		this->position = glm::vec3(position.x, terrain_height, position.z);
		//std::cout << "Cam_Pos y : " << this->position.y << std::endl; 
	}
	else {
		this->position = position;
	}
}

void Physic::detect_terrain_height(std::vector<std::vector<glm::vec3>> terrainXZ, glm::vec3 Cam_Pos)
{
	GLfloat x = Cam_Pos.x;
	GLfloat z = Cam_Pos.z;
	GLfloat scale_diff = terrain_size / (vertex_count-1);
	GLint x_to_map = x / (scale_diff);
	GLint z_to_map = z / (scale_diff);
	GLint vertices_Location = ((vertex_count-1) * z_to_map) + x_to_map;
	std::vector<glm::vec3> getSquare = terrainXZ[vertices_Location];
	GLfloat new_X = scale_diff - (Cam_Pos.z/ terrain_size);
	//std::cout << getSquare[0].y << " " << getSquare[1].y << " " << getSquare[2].y << " " << getSquare[3].y << std::endl;
	if (Cam_Pos.x >= (new_X)) {

		this->terrain_height = barryCentric(getSquare[1], getSquare[0], getSquare[2], Cam_Pos, scale_diff);
		
	}
	else if (Cam_Pos.x < new_X)
	{
		this->terrain_height = barryCentric(getSquare[1], getSquare[3], getSquare[2], Cam_Pos, scale_diff);
	}
	//std::cout << this->terrain_height << std::endl;
}

GLfloat Physic::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 Pos, GLfloat scale_diff)
{
	GLfloat det = ((p2.z - p3.z) * (p1.x - p3.x)) + ((p3.x - p2.x) * (p1.z - p3.z));
	GLfloat l1 = (((p2.z - p3.z) * (Pos.x - p3.x)) + ((p3.x - p2.x) * (Pos.z - p3.z))) / det;
	GLfloat l2 = (((p3.z - p1.z) * (Pos.x - p3.x)) + ((p1.x - p3.x) * (Pos.z - p3.z))) / det;
	GLfloat l3 = scale_diff - l1 - l2;
//	std::cout << l1 << " " << l2 << " " << l3 << std::endl;
	return ((l1 * p1.y) + (l2 * p2.y) + (l3 * p3.y))/ scale_diff;

}

void Physic::setTerrainSize(GLfloat terrainSize)
{
	this->terrain_size = terrainSize;
	//std::cout << terrain_size << std::endl;
}

void Physic::setVertexCount(GLfloat vertexCount)
{
	this->vertex_count = vertexCount;
	//std::cout << vertex_count << std::endl;
}