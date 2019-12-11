#include "Position_Geometry.h"


Position_Geometry::Position_Geometry()
{
}
Position_Geometry::~Position_Geometry()
{
}

void Position_Geometry::push_Geometries(Geometry* geometry, glm::vec3 xyz)
{
	this->geometry_on_position.push_back(make_pair(xyz, geometry));
}
vector<pair<glm::vec3, Geometry*>> Position_Geometry::get_Geometries()
{
	return this->geometry_on_position;
}
bool Position_Geometry::is_Empty()
{
	if (this->geometry_on_position.size() == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}