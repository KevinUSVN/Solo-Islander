#include "Terrain.h"
#include "stb-master/stb_image.h"

Terrain::Terrain(GLuint program)
{
	this->model = glm::mat4(1.0f);
	shaderProgram = program;

	std::cout << 1.0f / 2.0f * 4.0f << std::endl;

	initTerrain();
	setBuffers();
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
	//Set points (30 at y should be max height)
	for (int i = 0; i < vertex_count; i++)
	{
		for (int j = 0; j < vertex_count; j++)
		{
			//Set random number generator
			srand((unsigned int)time(NULL));

			//Setup the vertices.
			float vertex_x = ((float)j / ((float)vertex_count - 1)) * terrain_size;

			//Setup random height values (y-axis)
			float vertex_y = 0.0f;
			if (i == (vertex_count / 2)) //Currently at midpoint
			{
				vertex_y = rand() % 30;
			}
			if (j == (vertex_count / 2))
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * texCoords.size(), texCoords.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(2);

	//Set textures
	this->terrainTexture = loadTextures(list_textures);

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
	
	glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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

void Terrain::setupTextures()
{
	this->list_textures.push_back("./textures/terrain/water_tex1.jpg");
}

/* Load the texture and return a texture ID. */
GLuint Terrain::loadTextures(std::vector<std::string> list_textures)
{
	//Hold the textureID (This will be the textureID to return). textureID_1 holds the water texture.
	GLuint textureID_1;

	//Define variables to hold height map's width, height, pixel information.
	int width, height, nrChannels;
	unsigned char* data;

	//Create ID for texture.
	glGenTextures(1, &textureID_1);
	glActiveTexture(GL_TEXTURE0); //Set this texture to be the active texture (0).

	//Set this texture to be the one we are working with.
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_1);

	//Generate the texture.
	/*for (GLuint i = 0; i < list_textures.size(); i++)
	{
		unsigned char* data = stbi_load(list_textures[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}*/

	//Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Use bilinear interpolation:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Use clamp to edge to hide skybox edges:
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //X
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Y
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); //Z

	//Unbind the texture cube map.
	glBindTexture(GL_TEXTURE_2D, 0);
	//Return the textureID, we need to keep track of this texture variable.
	return textureID_1;
}

std::vector<std::vector<glm::vec3>> Terrain::getVertex()
{
	return this->square_location;
}