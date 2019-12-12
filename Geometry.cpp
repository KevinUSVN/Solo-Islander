#include "Geometry.h"


void Geometry::draw(glm::mat4 c, GLuint program)
{
	this->model = c * initial;
	programShader = program; //Set shader program
	
	setUniformColorVariable(program); //Send data to shader
	/*std::cout << "ambient : " << ambient.x << " " << ambient.y << " " << ambient.z << std::endl;
	std::cout << "specular : " << specular.x << " " << specular.y << " " << specular.z << std::endl;
	std::cout << "diffuse : " << diffuse.x << " " << diffuse.y << " " << diffuse.z << std::endl;
	std::cout << "color : " << color.x << " " << color.y << " " << color.z << std::endl;*/

	glBindVertexArray(vao);
	if (render == true) {
		if (render_in_triangle == true) {
			glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
		}
		if (render_in_lines == true)
		{
			glDrawElements(GL_LINES, vertexIndices.size(), GL_UNSIGNED_INT, 0);
		}
	}
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Geometry::Geometry(std::string objFilename, GLuint select)
{
	if (objFilename.substr(objFilename.find_last_of(".") + 1) == "obj") {
		std::ifstream objFile(objFilename); // The obj file we are reading.
		// Check whether the file can be opened.

		if (objFile.is_open())
		{
			std::string line; // A line in the file.

			// Read lines from the file.
			while (std::getline(objFile, line))
			{
				// Turn the line into a string stream for processing.
				std::stringstream ss;
				ss << line;

				// Read the first word of the line.
				std::string label;
				ss >> label;

				// If the line is about vertex (starting with a "v").
				if (label == "v")
				{
					// Read the later three float numbers and use them as the 
					// coordinates.
					glm::vec3 point;
					ss >> point.x >> point.y >> point.z;

					// Process the point. For example, you can save it to a.
					vertices.push_back(point);
					colors.push_back(glm::vec3(0.5f,0.0f,0.0f));
					//ambient.push_back(glm::vec3(0.0f));
					//diffuse.push_back(glm::vec3(0.0f));
					//specular.push_back(glm::vec3(0.0f));
				}
				else if (label == "vn")
				{
					glm::vec3 normal;
					ss >> normal.x >> normal.y >> normal.z;
					normals.push_back(normal);
				}
				else if (label == "vt")
				{
					glm::vec3 texture;
					ss >> texture.x >> texture.y;
					uvs.push_back(texture);
				}
				//Gathered first value, ignored rest (May change later)
				else if (label == "f")
				{
					std::string field1, field2, field3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

					//field 1: vertex, uv, normal
					ss >> field1 >> field2 >> field3;

					std::replace(field1.begin(), field1.end(), '/', ' ');
					std::stringstream temp_ss1(field1);
					temp_ss1 >> vertexIndex[0] >> uvIndex[0] >> normalIndex[0];

					std::replace(field2.begin(), field2.end(), '/', ' ');
					std::stringstream temp_ss2(field2);
					temp_ss2 >> vertexIndex[1] >> uvIndex[1] >> normalIndex[1];

					std::replace(field3.begin(), field3.end(), '/', ' ');
					std::stringstream temp_ss3(field3);
					temp_ss3 >> vertexIndex[2] >> uvIndex[2] >> normalIndex[2];
					
					//std::cout << normalIndex[0] << std::endl;

					vertexIndices.push_back(vertexIndex[0] - 1);
					vertexIndices.push_back(vertexIndex[1] - 1);
					vertexIndices.push_back(vertexIndex[2] - 1);
					uvIndices.push_back(uvIndex[0] - 1);
					uvIndices.push_back(uvIndex[1] - 1);
					uvIndices.push_back(uvIndex[2] - 1);
					normalIndices.push_back(normalIndex[0] - 1);
					normalIndices.push_back(normalIndex[1] - 1);
					normalIndices.push_back(normalIndex[2] - 1);
					colors[vertexIndex[0]-1] = current_color;
					colors[vertexIndex[1]-1] = current_color;
					colors[vertexIndex[2]-1] = current_color;
					//ambient[vertexIndex[0]-1] = current_ambient;
					//ambient[vertexIndex[1]-1] = current_ambient;
					//ambient[vertexIndex[2]-1] = current_ambient;
					//diffuse[vertexIndex[0]-1] = current_diffuse;
					//diffuse[vertexIndex[1]-1] = current_diffuse;
					//diffuse[vertexIndex[2]-1] = current_diffuse;
					//specular[vertexIndex[0]-1] = current_specular;
					//specular[vertexIndex[1]-1] = current_specular;
					//specular[vertexIndex[2]-1] = current_specular;
				}
				else if (label == "usemtl")
				{
					std::string color_option;
					ss >> color_option;
					set_Materials(color_option);
				}
			}
		}
		else
		{
			std::cerr << "Can't open the file " << objFilename << std::endl;
		}

		objFile.close();
	}

	/*
	 * Readjust to center and scale the model
	 */

	 //Initialized it to the first point in the file. 
	GLfloat minX = vertices[0].x;
	GLfloat maxX = vertices[0].x;
	GLfloat minY = vertices[0].y;
	GLfloat maxY = vertices[0].y;
	GLfloat minZ = vertices[0].z;
	GLfloat maxZ = vertices[0].z;
	GLfloat randX;
	GLfloat randY;
	GLfloat randZ;
	srand(time(0));
	//Finding min and max value of the points 
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i].x < minX)
		{
			minX = vertices[i].x;
		}
		if (vertices[i].x > maxX)
		{
			maxX = vertices[i].x;
		}
		if (vertices[i].y < minY)
		{
			minY = vertices[i].y;
		}
		if (vertices[i].y > maxY)
		{
			maxY = vertices[i].y;
		}
		if (vertices[i].z < minZ)
		{
			minZ = vertices[i].z;
		}
		if (vertices[i].z > maxZ)
		{
			maxZ = vertices[i].z;
		}
		randX = ((GLfloat(rand()) / GLfloat(RAND_MAX)) * (maxX - minX)) + minX;
		randY = ((GLfloat(rand()) / GLfloat(RAND_MAX)) * (maxY - minY)) + minY;
		randZ = ((GLfloat(rand()) / GLfloat(RAND_MAX)) * (maxZ - minZ)) + minZ;
		glm::vec3 dis_point;
		dis_point.x = randX;
		dis_point.y = randY;
		dis_point.z = randZ;
		initial_dis_points.push_back(dis_point);
	}
	// Finding the center of min and max value
	GLfloat midX = minX + abs((maxX - minX) / 2);
	GLfloat midY = minY + abs((maxY - minY) / 2);
	GLfloat midZ = minZ + abs((maxZ - minZ) / 2);

	max_x = maxX;
	min_x = minX;
	max_y = maxY;
	min_y = minY;
	max_z = maxZ;
	min_z = minZ;
	bounding_variables->push_back(glm::vec3(min_x, max_y, min_z));
	bounding_variables->push_back(glm::vec3(max_x, max_y, min_z));
	bounding_variables->push_back(glm::vec3(min_x, min_y, min_z));
	bounding_variables->push_back(glm::vec3(max_x, min_y, min_z));
	bounding_variables->push_back(glm::vec3(min_x, max_y, max_z));
	bounding_variables->push_back(glm::vec3(max_x, max_y, max_z));
	bounding_variables->push_back(glm::vec3(min_x, min_y, max_z));
	bounding_variables->push_back(glm::vec3(max_x, min_y, max_z));


	centerx = midX;
	centery = midY;
	centerz = midZ;

	// Subtract every value in Points by the center we found
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = vertices[i].x - midX;
		vertices[i].y = vertices[i].y - midY;
		vertices[i].z = vertices[i].z - midZ;
		initial_dis_points[i].x = initial_dis_points[i].x - midX;
		initial_dis_points[i].y = initial_dis_points[i].y - midY;
		initial_dis_points[i].z = initial_dis_points[i].z - midZ;

	}

	// Finding max distance from center point
	// initialize max distane to first points
	maxDistance = sqrt(((vertices[0].x), 2) + pow((vertices[0].y), 2) + pow((vertices[0].z), 2));
	GLfloat tempDist;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{	// Finding the distance to center point and compare to current max
		tempDist = sqrt(pow((vertices[i].x), 2) + pow((vertices[i].y), 2) + pow((vertices[i].z), 2));
		if (maxDistance < tempDist)
		{
			maxDistance = tempDist;
		}
	}
	GLfloat Scale = 13 / maxDistance;
	maxDistance = 13;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glm::vec3 scalar_point;
		if (vertices[i].x != initial_dis_points[i].x)
		{
			float distance_per_frame_x = (vertices[i].x - initial_dis_points[i].x) / 1000;
			scalar_point.x = distance_per_frame_x;
		}
		if (vertices[i].y != initial_dis_points[i].y)
		{
			float distance_per_frame_y = (vertices[i].y - initial_dis_points[i].y) / 1000;
			scalar_point.y = distance_per_frame_y;
		}
		if (vertices[i].z != initial_dis_points[i].z)
		{
			float distance_per_frame_z = (vertices[i].z - initial_dis_points[i].z) / 1000;
			scalar_point.z = distance_per_frame_z;
		}
		scalar_dis_points.push_back(scalar_point);
	}


	// Set the model matrix to an identity matrix. 
	initial = glm::scale(glm::mat4(1), glm::vec3(Scale));

	//Set colors for materials
	render = true;
	render_in_lines = false;
	render_in_triangle = true;
	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	// Bind to the VAO.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Bind to the first VBO. We will use it to store the points.
	glGenBuffers(3, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndices.size(),
		&vertexIndices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3 ) * normals.size(), &normals[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned int), 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* colors.size(), &colors[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned int), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ambient.size(), &ambient[0], GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned int), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * diffuse.size(), &diffuse[0], GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(5);
	//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned int), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * specular.size(), &specular[0], GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned int), 0);
	// Store index array in an element buffer


	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	glDeleteBuffers(1, &vbo[0]);
	glDeleteBuffers(1, &vbo[1]);
	glDeleteBuffers(1, &vbo[2]);
	glDeleteBuffers(1, &ebo);

	glDeleteVertexArrays(1, &vao);
}

void Geometry::update(glm::mat4 c)
{
}

void Geometry::setUniformVariable(GLuint program)
{

	// glUniform"type"(glgetUniformLocation(shader program, ""), 1,2, pointer);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
	//glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
	//glUniform3fv(glGetUniformLocation(program, "material.ambient"), 1, glm::value_ptr(ambient));
	//glUniform3fv(glGetUniformLocation(program, "material.diffuse"), 1, glm::value_ptr(diffuse));
	//glUniform3fv(glGetUniformLocation(program, "material.specular"), 1, glm::value_ptr(specular));
	//glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);
	//glUniform1f(glGetUniformLocation(program, "attenuationStrength"), 0.1f);
}

void Geometry::setUniformColorVariable(GLuint program)
{

	// glUniform"type"(glgetUniformLocation(shader program, ""), 1,2, pointer);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(program, "lightColor"), 1, glm::value_ptr(lightColor));
	//glUniform3fv(glGetUniformLocation(program, "material.ambient"), 1, glm::value_ptr(this->current_ambient));
	//glUniform3fv(glGetUniformLocation(program, "material.diffuse"), 1, glm::value_ptr(this->current_diffuse));
	//glUniform3fv(glGetUniformLocation(program, "material.specular"), 1, glm::value_ptr(this->current_specular));
	glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);
	glUniform1f(glGetUniformLocation(program, "attenuationStrength"), 0.1f); //0.1f default
}

void Geometry::setUniformTexture(GLuint program)
{
}

void Geometry::toggle_render()
{
	render = !render;
}

void Geometry::set_render_method(GLuint method)
{
	if (method == GL_LINES)
	{
		render_in_lines = true;
		render_in_triangle = false;
	}
	if (method == GL_TRIANGLES)
	{
		render_in_lines = false;
		render_in_triangle = true;
	}
}
void Geometry::set_Materials(std::string ColorOption)
{
	if (ColorOption == "Material.001") {
		current_color = glm::vec3(0.048f, 0.124f, 0.017f);
		current_ambient = glm::vec3(0.5f,0.5f,0.5f);
		current_diffuse = glm::vec3(0.048f,0.124f,0.017f);
		current_specular = glm::vec3(0.5f,0.5f,0.5f);
	}
	else if (ColorOption == "GreenLeaves")
	{
		current_color = glm::vec3(0.172f, 0.389f, 0.027f);
		current_ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		current_diffuse = glm::vec3(0.172f, 0.389f, 0.027f);
		current_specular = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (ColorOption == "Trunck")
	{
		current_color = glm::vec3(0.142f, 0.052f, 0.023f);
		current_ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		current_diffuse = glm::vec3(0.142f, 0.052f, 0.023f);
		current_specular = glm::vec3(0.076f, 0.076f, 0.076f);
	}
	else if (ColorOption == "Material.005")
	{
		current_color = glm::vec3(0.8f,0.8f,0.8f);
	}
	else if (ColorOption == "Material.002")
	{
		current_color = glm::vec3(0.8f, 0.622f, 0.015f);
	}
	else if (ColorOption == "blue")
	{
		current_color = glm::vec3(0.07f, 0.539f, 0.515f);
	}
	else if (ColorOption == "Material")
	{
		current_color = glm::vec3(0.8f, 0.065f, 0.018f);
	}
}

glm::mat4 Geometry::get_model()
{
	return this->model;
}


bool Geometry::getRender()
{
	return render;
}

void Geometry::scale(glm::vec3 scale_value)
{
	initial = glm::scale(glm::mat4(1), glm::vec3(scale_value));
}

void Geometry::set_render(bool option)
{
	this->render = option;
}

GLfloat Geometry::getMaxDistance()
{
	return maxDistance;
}

GLfloat Geometry::getMax_x()
{
	return max_x;
}

GLfloat Geometry::getMax_y()
{
	return max_y;

}

GLfloat Geometry::getMax_z()
{
	return max_z;

}

GLfloat Geometry::getMin_x()
{
	return min_x;

}

GLfloat Geometry::getMin_y()
{
	return min_y;
}

GLfloat Geometry::getMin_z()
{
	return min_z;
}