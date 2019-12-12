#include "Terrain.h"
#include "stb-master/stb_image.h"

Terrain::Terrain(GLuint program)
{
	this->model = glm::mat4(1.0f);
	shaderProgram = program;

	initTerrain(); // Creates a terrain using diamond square
	setBuffers();
	setupTextures();
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(3, this->vbo);
	glDeleteBuffers(1, &this->ebo);
}

/*
	Creates n points where n is the vertex_count

	Create points from x, y (random), and z values
	Also sets normals, texture coordinates, and indices (for triangle ordering)
*/
void Terrain::initTerrain()
{
	//Set random number generator
	srand((unsigned int)time(NULL));
	//Set points (30 at y should be max height)
	for (int i = 0; i < vertex_count; i++)
	{
		for (int j = 0; j < vertex_count; j++)
		{
			//Setup the vertices.
			float vertex_x = ((float)j / ((float)vertex_count - 1)) * terrain_size;

			//Setup random height values (y-axis)
			float vertex_y = 0.0f;
			int setGen1 = rand() % (int)vertex_count;
			int setGen2 = rand() % (int)vertex_count;

			if (i > setGen1) //Set as midpoint for island
			{
				vertex_y = rand() % 30;
			}
			if (j < setGen2)
			{
				vertex_y = rand() % 30;
			}
			
			float vertex_z = ((float)i / ((float)vertex_count - 1)) * terrain_size;


			//Setup the normals.
			float normal_x = 0;
			float normal_y = 1.0f;
			float normal_z = 0;

			//Setup the texcoords.
			float texCoord_x = (float)j / ((float)vertex_count - 1);
			float texCoord_y = (float)i / ((float)vertex_count - 1);

			//Push back to vectors.
			vertices.push_back(glm::vec3(vertex_x, vertex_y, vertex_z));
			normals.push_back(glm::vec3(normal_x, normal_y, normal_z));
			texCoords.push_back(glm::vec2(texCoord_x, texCoord_y));
		}
	}

	//Set indices (Order the vertices counter-clockwise for triangles)
	for (int i = 0; i < vertex_count - 1; i++)
	{
		for (int j = 0; j < vertex_count - 1; j++)
		{
			int topLeft = (i * vertex_count) + j; //0//1
			int topRight = topLeft + 1; //1//2
			int bottomLeft = ((i + 1) * vertex_count) + j; //128//129
			int bottomRight = bottomLeft + 1; //129//130

			//Push back to indices.
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}

	//Set random height using Diamond Square
	diamondSquare(0, vertex_count - 1, 0, vertex_count - 1, level, range);

	//Fix triangles around edges
	fixEdges();

	// Update heights in square_location vector
	for (int i = 0; i < vertex_count - 1; i++)
	{
		for (int j = 0; j < vertex_count - 1; j++)
		{
			int topLeft = (i * vertex_count) + j; //0//1
			int topRight = topLeft + 1; //1//2
			int bottomLeft = ((i + 1) * vertex_count) + j; //128//129
			int bottomRight = bottomLeft + 1; //129//130

			//Store the vertices in a temp vector. Temp vector is the "square"
			temp_vector.push_back(vertices[topLeft]);
			temp_vector.push_back(vertices[topRight]);
			temp_vector.push_back(vertices[bottomLeft]);
			temp_vector.push_back(vertices[bottomRight]);

			//Store each square (vector), square location contains all the "squares"
			square_location.push_back(temp_vector);

			temp_vector.clear(); //Clear temp vector for another set of vertices
		}
	}

	// Update normals
	updateNormals();
}

/*
	x1, x2, y1, y2 are top down view
	x1: begin horizontal point
	x2: end horizontal point

	y1: begin vertical point
	y2: end vertical point

	level: configures how 'wide' the "mountain"

	range: 
*/
void Terrain::diamondSquare(int x1, int x2, int y1, int y2, int level, float range)
{
	// Base Case
	if (level < 1)
		return;

	//Start random number generation.
	srand((unsigned int)time(NULL));

	//Diamond Algorithm
	for (int i = x1 + level; i < x2; i += level)
	{
		for (int j = y1 + level; j < y2; j += level)
		{
			//Get the 4 main vertices.
			glm::vec3 vec_a = this->vertices[((j - level) * vertex_count) + (i - level)];
			glm::vec3 vec_b = this->vertices[((j - level) * vertex_count) + i];
			glm::vec3 vec_c = this->vertices[(j * vertex_count) + (i - level)];
			glm::vec3 vec_d = this->vertices[(j * vertex_count) + i];

			//Get the 4 heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			float height_d = vec_d.y;

			//Get the middle vertex.
			glm::vec3 vec_e = this->vertices[((j - level / 2) * vertex_count) + (i - level / 2)];

			//Calculate the average height in the middle and set it to E.
			float height_e = (float)(height_a + height_b + height_c + height_d) / 4; // Averages of the four corner points heights
			height_e += fmod(((float)(rand()) / 1000), max_displacement) * range; //Add random value
			vec_e.y = height_e; // Modify middle point y-value
			this->vertices[((j - level / 2) * vertex_count) + (i - level / 2)] = vec_e; // Update the middle point value
		}
	}

	//Square algorithm
	for (int i = x1 + 2 * level; i < x2; i += level)
	{
		for (int j = y1 + 2 * level; j < y2; j += level)
		{
			//Get the 4 main vertices.
			glm::vec3 vec_a = this->vertices[((j - level) * vertex_count) + (i - level)];
			glm::vec3 vec_b = this->vertices[((j - level) * vertex_count) + i];
			glm::vec3 vec_c = this->vertices[(j * vertex_count) + (i - level)];
			glm::vec3 vec_d = this->vertices[(j * vertex_count) + i];

			//Get the 4 main heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			float height_d = vec_d.y;
			

			//Get the middle vertex.
			glm::vec3 vec_e = this->vertices[((j - level / 2) * vertex_count) + (i - level / 2)];

			//Get the middle vertex height
			float height_e = vec_e.y;

			//Calculate the average height and set it to F.
			glm::vec3 vec_f = this->vertices[((j - level / 2) * vertex_count) + (i - level)]; //

			float height_f = (float)(height_a + height_c + height_e
				+ this->vertices[((j - level / 2) * vertex_count) + (i - 3 * level / 2)].y) / 3; //Average three adjacent points

			height_f += fmod(((float)(rand()) / 1000), max_displacement) * range;
			vec_f.y = height_f; // Modify
			this->vertices[((j - level / 2) * vertex_count) + (i - level)] = vec_f; // Update

			//Calculate the average height and set it to G.
			glm::vec3 vec_g = this->vertices[((j - level) * vertex_count) + (i - level / 2)];

			float height_g = (float)(height_a + height_b + height_e
				+ this->vertices[((j - 3 * level / 2) * vertex_count) + (i - level / 2)].y) / 3; //Average three adjacent points

			height_g += fmod(((float)(rand()) / 1000), max_displacement) * range;
			vec_g.y = height_g; // Modify
			this->vertices[((j - level) * vertex_count) + (i - level / 2)] = vec_g; // Update
		}
	}
	//Begin Recursion.
	diamondSquare(x1, x2, y1, y2, level / 2, range / 2);
}

void Terrain::setBuffers()
{
	/*
		Surface
	*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, vbo);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	// Bind to the EBO.
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //Bind indices buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//Bind for normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	//Bind for texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(2);
	
	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Terrain::draw()
{
	// Draw terrain
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	
	// Set active textures
	glActiveTexture(GL_TEXTURE0); // 0 as default, water
	glBindTexture(GL_TEXTURE_2D, this->terrainTexture1);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain_water"), 0);

	glActiveTexture(GL_TEXTURE1); // sand
	glBindTexture(GL_TEXTURE_2D, this->terrainTexture2);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain_sand"), 1);

	glActiveTexture(GL_TEXTURE2); // grass
	glBindTexture(GL_TEXTURE_2D, this->terrainTexture3);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain_grass"), 2);

	glActiveTexture(GL_TEXTURE3); // snow
	glBindTexture(GL_TEXTURE_2D, this->terrainTexture4);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain_snow"), 3);

	//Add additional textures here
	/* --- Example Case ---
		glActiveTexture(GL_TEXTURE1);//Enable the texture.
		glBindTexture(GL_TEXTURE_2D, this->terrainTexture_1);
		glUniform1i(glGetUniformLocation(shaderProgram, "TerrainTexture_1"), 1);
	*/


	glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/* Load the texture and return a texture ID. */
GLuint Terrain::loadTexture(std::string filePath)
{
	//Hold the textureID (This will be the textureID to return). textureID holds the water texture.
	GLuint textureID;

	//Define variables to hold height map's width, height, pixel information.
	int width, height, nrChannels;
	unsigned char* data;

	//Create ID for texture.
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0); //Set this texture to be the active texture (0).

	//Set this texture to be the one we are working with.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture file
	data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Unbind the texture cube map.
	glBindTexture(GL_TEXTURE_2D, 0);
	//Return the textureID, we need to keep track of this texture variable.
	return textureID;
}

/*
	Loads the textures from file and obtain texture ID
*/
void Terrain::setupTextures()
{
	this->terrainTexture1 = loadTexture("textures/terrain/water.jpg");
	this->terrainTexture2 = loadTexture("textures/terrain/sand.jpg");
	this->terrainTexture3 = loadTexture("textures/terrain/grass.jpg");
	this->terrainTexture4 = loadTexture("textures/terrain/snow.jpg");
}

std::vector<std::vector<glm::vec3>> Terrain::getVertex()
{
	return this->square_location;
}

/* Return the height at a given x, y coordinate. */
float Terrain::getHeightFromVertex(int x, int y)
{
	if ((x < 0) || (x >= vertex_count) || (y < 0) || (y >= vertex_count))
	{
		return 0;
	}
	return this->vertices[(y * vertex_count) + x].y;
}

/* Gets the size of the terrain (should be a power of 2) */
GLfloat Terrain::getTerrainSize()
{
	return terrain_size;
}

/* Gets the vertex count of the terrain */
GLfloat Terrain::getVertexCount()
{
	return vertex_count;
}

/* Updates the normals for the entire terrain. */
void Terrain::updateNormals()
{
	for (int i = 0; i < vertex_count; i++)
	{
		for (int j = 0; j < vertex_count; j++)
		{
			//Get the updated heights after diamond-square modification
			float heightL = getHeightFromVertex(j - 1, i);
			float heightR = getHeightFromVertex(j + 1, i);
			float heightD = getHeightFromVertex(j, i + 1);
			float heightU = getHeightFromVertex(j, i - 1);

			//Update the normal.
			glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 1.0f, heightU - heightD));
			this->normals[(i * vertex_count) + j] = normal;
		}
	}
}

/*
	Fixes triangle popups around the edges of the terrain
	Update the height values of the edges to be zero
*/
void Terrain::fixEdges()
{
	for (int i = 0; i < vertex_count; i++)
	{
		//Retain x-values and z-values
		float x_axis_top = vertices[i].x;
		float z_axis_top = vertices[i].z;

		float x_axis_bottom = vertices[(vertex_count * (vertex_count - 1)) + i].x;
		float z_axis_bottom = vertices[(vertex_count * (vertex_count - 1)) + i].z;

		float x_axis_left = vertices[i * vertex_count].x;
		float z_axis_left = vertices[i * vertex_count].z;

		float x_axis_right = vertices[(i * vertex_count) + (vertex_count - 1)].x;
		float z_axis_right = vertices[(i * vertex_count) + (vertex_count - 1)].z;

		//Update the edges.
		glm::vec3 vertex_top = glm::vec3(x_axis_top, 0.0f, z_axis_top);
		glm::vec3 vertex_bottom = glm::vec3(x_axis_bottom, 0.0f, z_axis_bottom);
		glm::vec3 vertex_left = glm::vec3(x_axis_left, 0.0f, z_axis_left);
		glm::vec3 vertex_right = glm::vec3(x_axis_right, 0.0f, z_axis_right);

		this->vertices[i] = vertex_top;
		this->vertices[(vertex_count * (vertex_count - 1)) + i] = vertex_bottom;
		this->vertices[i * vertex_count] = vertex_left;
		this->vertices[(i * vertex_count) + (vertex_count - 1)] = vertex_right;
	}
}

unsigned int Terrain::getSquareAmount()
{
	return this->square_location.size();
}


void Terrain::update()
{
	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();
	square_location.clear();
	temp_vector.clear();

	initTerrain();
	setBuffers();
	draw();
}

GLfloat Terrain::find_terrain_height(glm::vec3 Cam_Pos)
{
	GLfloat x = Cam_Pos.x;
	GLfloat z = Cam_Pos.z;
	GLfloat scale_diff = this->terrain_size / (this->vertex_count-1);
	GLint x_to_map = x / (scale_diff);
	GLint z_to_map = z / (scale_diff);
	//std::cout << "Z:" << z_to_map << "X: " << x_to_map  << " " << this->vertex_count-1 << std::endl;
	GLint vertices_Location = ((this->vertex_count-1) * (z_to_map)) + (x_to_map);
	std::vector<glm::vec3> getSquare = square_location[vertices_Location];
	GLfloat new_X = scale_diff - (Cam_Pos.z/this->terrain_size);
	//std::cout << getSquare[0].y << " " << getSquare[1].y << " " << getSquare[2].y << " " << getSquare[3].y << std::endl;
	if (Cam_Pos.x >= (new_X)) {

		return barryCentric(getSquare[1], getSquare[0], getSquare[2], Cam_Pos, scale_diff);

	}
	else if (Cam_Pos.x < new_X)
	{
		return barryCentric(getSquare[1], getSquare[3], getSquare[2], Cam_Pos, scale_diff);
	}
	//std::cout << this->terrain_height << std::endl;
}

GLfloat Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 Pos, GLfloat scale_diff)
{
	GLfloat det = ((p2.z - p3.z) * (p1.x - p3.x)) + ((p3.x - p2.x) * (p1.z - p3.z));
	GLfloat l1 = (((p2.z - p3.z) * (Pos.x - p3.x)) + ((p3.x - p2.x) * (Pos.z - p3.z))) / det;
	GLfloat l2 = (((p3.z - p1.z) * (Pos.x - p3.x)) + ((p1.x - p3.x) * (Pos.z - p3.z))) / det;
	GLfloat l3 = scale_diff - l1 - l2;
	//	std::cout << l1 << " " << l2 << " " << l3 << std::endl;
	return ((l1 * p1.y) + (l2 * p2.y) + (l3 * p3.y)) / scale_diff;

}