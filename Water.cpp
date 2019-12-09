#include "Water.h"
#include "stb-master/stb_image.h"

Water::Water(GLuint program)
{
	this->model = glm::mat4(1.0f);
	this->shaderProgram = program;

	initWater();
	setBuffers(); //To draw water plane

	//Get reflection and refraction textures
	setReflectionFrameBuffer();
	setRefractionFrameBuffer();

	setupTextures();
}

Water::~Water()
{
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(3, this->vbo);
	glDeleteBuffers(1, &this->ebo);
}

void Water::initWater()
{
	//Set points as flat plane
	for (int i = 0; i < vertex_count; i++)
	{
		for (int j = 0; j < vertex_count; j++)
		{
			//Setup the vertices.
			float vertex_x = ((float)j / ((float)vertex_count - 1)) * plane_size;

			//Setup random height values (y-axis)
			float vertex_y = 10.0f;

			float vertex_z = ((float)i / ((float)vertex_count - 1)) * plane_size;


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
}

void Water::setBuffers()
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

/*
	Render the water
*/
void Water::draw()
{
	//Set wave speed
	//move_factor = move_factor + (wave_speed * (abs(diff_sec) / 10000));
	move_factor = move_factor + wave_speed;

	//std::cout << "Move factor: " << move_factor << std::endl;

	// Draw terrain
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(shaderProgram, "moveFactor"), move_factor);

	glBindVertexArray(vao);

	// Set active textures
	glActiveTexture(GL_TEXTURE0); // 0 as default, water
	glBindTexture(GL_TEXTURE_2D, this->waterTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "terrain_water"), 0);

	glActiveTexture(GL_TEXTURE1); // 0 as default, water
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "cubeMap"), 1);

	glActiveTexture(GL_TEXTURE2); // 0 as default, water
	glBindTexture(GL_TEXTURE_2D, this->dudvMap);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudvMap"), 2);

	glActiveTexture(GL_TEXTURE3); // 0 as default, water
	glBindTexture(GL_TEXTURE_2D, this->reflectionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 3);

	glActiveTexture(GL_TEXTURE4); // 0 as default, water
	glBindTexture(GL_TEXTURE_2D, this->refractionTexture);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 4);

	//Add additional textures here
	/* --- Example Case ---
		glActiveTexture(GL_TEXTURE1);//Enable the texture.
		glBindTexture(GL_TEXTURE_2D, this->terrainTexture_1);
		glUniform1i(glGetUniformLocation(shaderProgram, "TerrainTexture_1"), 1);
	*/

	glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Water::update()
{
	vertices.clear();
	normals.clear();
	texCoords.clear();
	indices.clear();
	square_location.clear();
	temp_vector.clear();

	initWater();
	setBuffers();
	draw();
}

std::vector<std::vector<glm::vec3>> Water::getVertex()
{
	return this->square_location;
}

void Water::setupTextures()
{
	this->waterTexture = loadTexture("textures/terrain/water.jpg");
	this->dudvMap = loadTexture("textures/water/waterDUDV.png");
}

GLuint Water::loadTexture(std::string filePath)
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

void Water::setCubeMap(GLuint textureID)
{
	skyboxTexture = textureID;
}

/*
	Create Frame Buffer Object for reflection
*/
void Water::setReflectionFrameBuffer()
{
	/*
		Creating FBO for reflection
	*/
	glGenFramebuffers(1, &this->reflection_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->reflection_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	/*
		Color attachment reflection texture
	*/
	glGenTextures(1, &this->reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, this->reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, reflection_width, reflection_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);

	/*
		Reflection Depth Buffer
	*/
	glGenRenderbuffers(1, &reflection_dbo);
	glBindRenderbuffer(GL_RENDERBUFFER, reflection_dbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, reflection_width, reflection_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflection_dbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cerr << "Water::initFrameBuffers - Failed to create reflection frame buffer" << std::endl;

	//Unbind all
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
	Create Frame Buffer Object for refraction
*/
void Water::setRefractionFrameBuffer()
{
	/*
		Creating FBO for refraction
	*/
	glGenFramebuffers(1, &this->refraction_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->refraction_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	/*
		Color attachment refraction texture
	*/
	glGenTextures(1, &this->refractionTexture);
	glBindTexture(GL_TEXTURE_2D, this->refractionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, refraction_width, refraction_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTexture, 0);

	/*
		Refraction Depth Texture
	*/
	glGenTextures(1, &this->refractionDepthTexture);
	glBindTexture(GL_TEXTURE_2D, this->refractionDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, refraction_width, refraction_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, refractionDepthTexture, 0);

	//Unbind all
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
	Send render data to reflectionBFO
*/
void Water::bindReflectionFrameBuffer()
{
	bindFrameBuffer(reflection_fbo, reflection_width, reflection_height);
}

/*
	Send render data to refractionFBO
*/
void Water::bindRefractionFrameBuffer()
{
	bindFrameBuffer(refraction_fbo, refraction_width, refraction_height);
}

void Water::bindFrameBuffer(GLuint FBO, float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, width, height);
}

/*
	Render back to default frame buffer object
*/
void Water::unbindCurrentFrameBuffer(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}


GLuint Water::getReflectionTexture()
{
	return this->reflectionTexture;
}

GLuint Water::getRefractionTexture()
{
	return this->refractionTexture;
}

GLuint Water::getReflectionFBO()
{
	return this->reflection_fbo;
}

float Water::getReflectionWidth()
{
	return this->reflection_width;
}

float Water::getReflectionHeight()
{
	return this->reflection_height;
}