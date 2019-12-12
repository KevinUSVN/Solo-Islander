#include "Window.h"

int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";
/*
	Declare any enum here.
*/
enum PLAYER { body = 0, left_hand = 1, right_hand = 2, head = 3 };

/*
	Initialize drawing node here
*/
Node* Window::currentNode;

/*
	Initialize environmental object here
*/
Skybox* Window::skybox;
glm::vec3 Window::lightPos = glm::vec3(0.0f,100.0f,0.0f);
Terrain* terrain;
Water* water;
vector<Position_Geometry*> geometry_on_pos;

/*
	Initialize all the Transformation here
*/
glm::vec3 playermovement = glm::vec3(0.0f, 0.0f, 0.0f);
Transform* Window::world_T_matrix;
Transform* bunnyT;
Transform* player_T;
Transform* player_right_hand_T;
Transform* player_left_hand_T;
Transform* testing_ground_T;
Transform* world_physic_T;
vector<Transform*> beach_hut_T;
vector<Transform*> trees_pos_T;
vector<Transform*> misc_T;

/*
	Initialize all the scene object here
*/
Geometry* light_object;
vector<Shape_Program*> beach_hut;
vector<Geometry*> trees;
vector<Geometry*> misc;


/*
	Initialize all the player object here
*/
Geometry* player;
Geometry* Player_right_hand;
Geometry* Player_left_hand;


/*
	Initialize all the shader program variable here
*/
GLuint Window::normal_color_program;
GLuint Window::program;
GLuint Window::default_program;
GLuint Window::skybox_program;
GLuint Window::texture_program;
GLuint Window::line_program;
GLuint Window::point_program;
GLuint Window::handle_program;
GLuint Window::terrain_program;
GLuint Window::water_program;
GLuint Window::toon_program;


/*
	Initialize all the boolean statement here
*/
bool boundary_on = false;
bool rotate_press = false;
bool normal_on = false;
bool toggle_on = false;
bool culling_toggle = false;
bool mode_one = true;
bool mode_two = false;
bool mode_three = false;
bool mode_four = false;
bool move_camera = true;
bool change_control_point = false; 
bool firstMouse;
bool cart_mode = false;
bool physic_mode = false;
bool pause = false;
int Point_index = 0;
bool alternator_true = true;
bool alternator_false = true;
bool toon_on = true;
bool water_on = true;

/*
	Initialize all the time variable here
*/
float last_time = 0.0f;
float current_time = 0.0f;
GLfloat currentSoundTime = 0.0f;
GLfloat soundInterval = 50.0f; // Define sound delay, lower = faster, higher = slower


/*
	Initialize all the camera variable here
*/
glm::vec3 Window::last_mouse_pos;
glm::vec3 Window::current_mouse_pos;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::Cam_target(0, 0, -1);
glm::vec3 Window::Cam_Pos(500.0f, 0, 500.0f); // Camera position.
glm::vec3 Window::Prev_Cam_Pos(0, 0, 0.0f);
glm::vec3 Window::Cam_Dir = glm::normalize(Cam_Pos + Cam_target);// The point we are looking at.
glm::vec3 Window::right = glm::normalize(glm::cross(Cam_target, glm::vec3(0.0f, 1.0f, 0.0f)));
glm::vec3 Window::up = glm::normalize(glm::cross(right, Cam_target)); // The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::Cam_Pos, Window::Cam_Dir, Window::up);
float angle = 0.0f;
float animation_dir = 1.0f;;
float animation_counter = 0.0f;
double fov = 45;
double old_fov = 45;
double new_fov = 120;
double frustum_fov = fov;
GLfloat sensitivity = 0.05;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat Camera_Speed_C = 75.0f;
GLfloat lastX = Window::width / 2.0f, lastY = Window::height / 2.0f;
double yaw = -90.0, pitch = 0.0;
glm::vec3 gravity_force = glm::vec3(0.0f, -10.0f, 0.0f);
Physic* gravity = new Physic(current_time, gravity_force, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,2.0f));
/*
	Initialize all the frustum variable here
*/
Plane* pl[6];
glm::vec3 fc;
glm::vec3 ftr;
glm::vec3 ftl;
glm::vec3 fbl;
glm::vec3 fbr;
glm::vec3 nc;
glm::vec3 ntl;
glm::vec3 ntr;
glm::vec3 nbl;
glm::vec3 nbr;

/*
	Initialize destructor here
*/
std::vector<Node*> Window::deleteTracker;


/*
	Initialize Audio (WARNING: DEBUG MODE will make sounds delay more longer, use release for accurate playback)
*/
irrklang::ISoundEngine* soundObject;
irrklang::ISoundEngine* ambientObject;
irrklang::ISoundEngine* ambient3DObject;

/*
	Initialize all shader programs in this function.
*/
bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	default_program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	normal_color_program = LoadShaders("shaders/shader_normal.vert", "shaders/shader_normal.frag");
	skybox_program = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	texture_program = LoadShaders("shaders/cubemaps.vert", "shaders/cubemaps.frag");
	line_program = LoadShaders("shaders/line_shader.vert", "shaders/line_shader.frag");
	point_program = LoadShaders("shaders/point_shader.vert", "shaders/point_shader.frag");
	handle_program = LoadShaders("shaders/handle_shader.vert", "shaders/handle_shader.frag");
	terrain_program = LoadShaders("shaders/shader_terrain.vert", "shaders/shader_terrain.frag");
	water_program = LoadShaders("shaders/shader_water.vert", "shaders/shader_water.frag");
	toon_program = LoadShaders("shaders/shader_toon.vert", "shaders/shader_toon.frag");
	// Check the shader program.
	if (!default_program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!normal_color_program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	// Activate the shader program.
	// Get the locations of uniform variables.

	return true;
}
/*
	Initialize all objects in this function.
*/
bool Window::initializeObjects()
{
	//Environment object
	skybox = new Skybox();
	terrain = new Terrain(terrain_program);
	water = new Water(water_program);

	unsigned int square_amount = terrain->getSquareAmount();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 6);
	for (unsigned int i = 0; i < square_amount; i++)
	{
		Position_Geometry* new_geo = new Position_Geometry();
		geometry_on_pos.push_back(new_geo);
	}
	for (unsigned int i = 0; i < 300; i++)
	{
		unsigned int k;
		k = dis(gen);
		switch (k)
		{
		case 0:
			trees.push_back(new Geometry ("OBJ_files/afro_tree_green.obj",1));
			break;
		case 1:
			trees.push_back(new Geometry("OBJ_files/big_tree_green.obj", 1));
			break;
		case 2:
			trees.push_back(new Geometry("OBJ_files/hipster_tree_green.obj", 1));
			break;
		case 3:
			trees.push_back(new Geometry("OBJ_files/pine_tree_green.obj", 1));
			break;
		case 4:
			trees.push_back(new Geometry("OBJ_files/slim_tree_green.obj", 1));
			break;
		case 5:
			trees.push_back(new Geometry("OBJ_files/three_balls_tree_green.obj", 1));
			break;
		case 6:
			trees.push_back(new Geometry("OBJ_files/tiny_tree_green.obj", 1));
			break;
		}
	}
	std::uniform_int_distribution<> misc_dis(0, 6);
	for (unsigned int i = 0; i < 300; i++)
	{
		unsigned int k;
		k = misc_dis(gen);
		switch (k)
		{
		case 0:
			misc.push_back(new Geometry("OBJ_files/blue_flower.obj", 1));
			break;
		case 1:
			misc.push_back(new Geometry("OBJ_files/red_flower.obj", 1));
			break;
		case 2:
			misc.push_back(new Geometry("OBJ_files/yellow_flower.obj", 1));
			break;
		case 3:
			misc.push_back(new Geometry("OBJ_files/green_grass.obj", 1));
			break;
		case 4:
			misc.push_back(new Geometry("OBJ_files/green_round_bush.obj", 1));
			break;
		case 5:
			misc.push_back(new Geometry("OBJ_files/green_small_bush.obj", 1));
			break;
		case 6:
			misc.push_back(new Geometry("OBJ_files/green_spikey_bush.obj", 1));
			break;
		}

	}

	//Send cubeMap data to Water class
	water->setCubeMap(skybox->getCubeMap());

	//Send terrain size and vertex count data to Physic class
	gravity->setTerrainSize(terrain->getTerrainSize());
	gravity->setVertexCount(terrain->getVertexCount());

	//Scenic Object

	//Player Object

	player = new Geometry("OBJ_files/square.obj",1);
	Player_right_hand = new Geometry("OBJ_files/RiggedHandRight.obj",1);
	Player_left_hand = new Geometry("OBJ_files/RiggedHandLeft.obj",1);

	//Audio Object

	soundObject = irrklang::createIrrKlangDevice();
	ambientObject = irrklang::createIrrKlangDevice();
	ambient3DObject = irrklang::createIrrKlangDevice();

	//Play Background Sounds
	ambientObject->play2D("audio/beach_ambience.wav", GL_TRUE);
	ambientObject->setSoundVolume(0.3f);

	//Play Environmental Sounds (3D)
	//ambient3DObject->play3D("audio/water_fountain.wav", irrklang::vec3df(0.0f, 0.0f, 0.0f), GL_TRUE);
	//ambient3DObject->setSoundVolume(0.3f);
	srand(time(NULL));

	for (unsigned int i = 0; i < 3; i++)
	{
		while (1)
		{
			unsigned int rand_x, rand_z;
			rand_x = rand() % ((int)(terrain->getTerrainSize())-50) + 50;
			rand_z = rand() % ((int)(terrain->getTerrainSize())-50) + 50;
			glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
			GLfloat new_height = terrain->find_terrain_height(rand_pos);
			if (new_height > 40.0f && new_height < 85.0f )
			{
				beach_hut_T.push_back(new Transform(glm::translate(glm::mat4(1), glm::vec3(rand_x, (new_height + 6.0f), rand_z))));
				break;
			}
		}
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		beach_hut.push_back(new Shape_Program("OBJ_files/testing.txt", beach_hut_T[i]));
	}
	return true;
}
/*
	Initialize all transformation in this function.
*/
bool Window::initializeTransforms() {
	//global transform
	world_T_matrix = new Transform(glm::mat4(1));
	world_physic_T = new Transform(glm::mat4(1));
	//Initialize physic

	//Object transform
	for (unsigned int i = 0; i < 3; i++)
	{
		beach_hut_T[i] = new Transform(glm::mat4(1));
	}

	//Player Transform
	player_T = new Transform(glm::mat4(1));
	player_right_hand_T = new Transform(glm::translate(glm::mat4(1),glm::vec3(-1.0f, 0.0f, -2.0f)));
	player_left_hand_T = new Transform(glm::translate(glm::mat4(1),glm::vec3(1.0f, 0.0f, -2.0f)));
	//Environment transform
	srand(time(NULL));
	for (unsigned int i = 0; i < 300; i++)
	{
		while (1)
		{
			unsigned int rand_x, rand_z;
			rand_x = rand() % (int)(terrain->getTerrainSize());
			rand_z = rand() % (int)(terrain->getTerrainSize());
			glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
			GLfloat new_height = terrain->find_terrain_height(rand_pos);
			if (new_height > 15.0f && new_height < 100.0f)
			{
				trees_pos_T.push_back(new Transform(glm::translate(glm::mat4(1),glm::vec3(rand_x, (new_height + (abs(trees[i]->getMax_y()-trees[i]->getMin_y())/2)+5.0f), rand_z))));
				break;
			}
		}
	}
	for (unsigned int i = 0; i < 300; i++)
	{
		while (1)
		{
			unsigned int rand_x, rand_z;
			rand_x = rand() % (int)(terrain->getTerrainSize());
			rand_z = rand() % (int)(terrain->getTerrainSize());
			glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
			GLfloat new_height = terrain->find_terrain_height(rand_pos);
			if (new_height > 15.0f && new_height < 100.0f)
			{
				misc_T.push_back(new Transform(glm::translate(glm::mat4(1), glm::vec3(rand_x, (new_height + (abs(misc[i]->getMax_y() - misc[i]->getMin_y()) / 2) + 5.0f), rand_z))));
				break;
			}
		}
	}

	currentNode = world_T_matrix;
	return true;
}
/*
	Apply any transform tree structure here.
*/
bool Window::applyTransforms() {
	//Global
	world_physic_T->addChild(player_T);
	for (unsigned int i = 0; i < 300; i++)
	{
		world_T_matrix->addChild(trees_pos_T[i]);
		trees_pos_T[i]->addChild(trees[i]);
		world_T_matrix->addChild(misc_T[i]);
		misc_T[i]->addChild(misc[i]);
	}
	//Environmental

	//Scenic
	//Player
	player_T->addChild(player);
	player_T->addChild(player_right_hand_T);
	player_T->addChild(player_left_hand_T);

	player_right_hand_T->addChild(Player_right_hand);
	player_left_hand_T->addChild(Player_left_hand);

	return true;
}

void Window::cleanUp()
{
	for (auto node : deleteTracker)
	{
		delete node;
	}

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0),
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	currentSoundTime--;
}

void Window::displayCallback(GLFWwindow* window)
{
	GLfloat new_time =  glfwGetTime();
	gravity->detect_terrain_height(terrain->getVertex(), Cam_Pos);
	Cam_Pos = glm::vec3(Cam_Pos.x,gravity->calculate_new_Pos(new_time).y+6.0f, Cam_Pos.z); //Player Position
	projection = glm::perspective(glm::radians(fov), double(width) / (double)height, 1.0, 1400.0);
	view = glm::lookAt(Window::Cam_Pos, Window::Cam_Pos + Window::Cam_target, Window::up);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw the terrain
	glUseProgram(terrain_program);

	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(terrain_program, "viewPos"), Cam_Pos.x, Cam_Pos.y, Cam_Pos.z);
	glUniform3f(glGetUniformLocation(terrain_program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(terrain_program, "toon_on"), toon_on);
	terrain->draw();
	//Draw the water
	//Draw the terrain
	glUseProgram(water_program);

	glUniformMatrix4fv(glGetUniformLocation(water_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(water_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(water_program, "viewPos"), Cam_Pos.x, Cam_Pos.y, Cam_Pos.z);
	glUniform3f(glGetUniformLocation(water_program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(water_program, "water_on"), water_on);
	water->draw();

	glUseProgram(default_program);
	glUniform1i(glGetUniformLocation(default_program, "skybox"), 0);
	currentNode->draw(glm::mat4(1), default_program);
	glUniformMatrix4fv(glGetUniformLocation(default_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(default_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(default_program, "cameraPos"), Cam_Pos.x, Cam_Pos.y, Cam_Pos.z);
	//Draw the skybox
	for (unsigned int i = 0; i < 3; i++) {
		beach_hut[i]->draw(glm::mat4(1), default_program);
	}
	//player_T->draw(glm::inverse(view), default_program);

	glUseProgram(skybox_program);
	//View matrix got changed.
	view = glm::mat4(glm::mat3(view));
	glUniform1i(glGetUniformLocation(skybox_program, "skybox"), 0);
	glUniformMatrix4fv(glGetUniformLocation(skybox_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skybox_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	skybox->drawSkybox();


	//Draw the lines
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_N:
			soundObject->play2D("audio/chop.wav", GL_FALSE);
			if (toon_on)
			{
				//program = normal_color_program;
				toon_on = !toon_on;
			}
			else
			{
				//program = default_program;
				toon_on = !toon_on;
			}
			break;
		case GLFW_KEY_M:
			soundObject->play2D("audio/chop.wav", GL_FALSE);
			water_on = !water_on;
			break;
		case GLFW_KEY_V:
			move_camera = true;
			change_control_point = false;
			cart_mode = false;
			break;

		case GLFW_KEY_C:
			cart_mode = true;
			move_camera = false;
			break;
		case GLFW_KEY_P:
			pause = !pause;
			break;
		case GLFW_KEY_F:
			physic_mode = !physic_mode;
			std::cout << "physic mode : " << physic_mode << endl;
			break;
		case GLFW_KEY_1:
			std::cout << "Key 1 Enable" << std::endl;
			mode_one = true;
			mode_two = false;
			mode_three = false;
			break;
		case GLFW_KEY_2:
			std::cout << "Key 2 Enable" << std::endl;
			mode_one = false;
			mode_two = true;
			mode_three = false;
			break;
		case GLFW_KEY_3:
			std::cout << "Key 3 Enable" << std::endl;
			glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
			mode_one = false;
			mode_two = false;
			mode_three = true;
			break;
		case GLFW_KEY_4:
			mode_one = false;
			mode_two = false;
			mode_three = true;
			mode_four = true;
			break;
		case GLFW_KEY_T:
			toggle_on = !toggle_on;
			if (toggle_on) {
				fov = 45.0f;
				frustum_fov = fov;
			}
			else {
				frustum_fov = frustum_fov;
				fov = fov + 100.0f;
			}
			break;
		case GLFW_KEY_R:
			soundObject->play2D("audio/chop.wav", GL_FALSE);
			terrain->update();
			for (unsigned int i = 0; i < 3; i++)
			{
				while (1)
				{
					unsigned int rand_x, rand_z;
					rand_x = rand() % ((int)(terrain->getTerrainSize()) - 50) + 50;
					rand_z = rand() % ((int)(terrain->getTerrainSize()) - 50) + 50;
					glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
					GLfloat new_height = terrain->find_terrain_height(rand_pos);
					if (new_height > 40.0f && new_height < 85.0f)
					{
						beach_hut_T[i] = new Transform(glm::translate(glm::vec3(rand_x, (new_height + 6.0f), rand_z)));
						beach_hut[i]->updateHeadTransform(beach_hut_T[i]);
						break;
					}
				}
			}
			for (unsigned int i = 0; i < 300; i++)
			{
				while (1)
				{
					unsigned int rand_x, rand_z;
					rand_x = rand() % (int)(terrain->getTerrainSize());
					rand_z = rand() % (int)(terrain->getTerrainSize());
					glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
					GLfloat new_height = terrain->find_terrain_height(rand_pos);
					if (new_height > 15.0f && new_height < 100.0f)
					{
						trees_pos_T[i]->update(glm::inverse(trees_pos_T[i]->getTransform()) *glm::translate(glm::mat4(1), glm::vec3(rand_x, (new_height + 6.0f), rand_z)));
						break;
					}
				}
			}
			for (unsigned int i = 0; i < 300; i++)
			{
				while (1)
				{
					unsigned int rand_x, rand_z;
					rand_x = rand() % (int)(terrain->getTerrainSize());
					rand_z = rand() % (int)(terrain->getTerrainSize());
					glm::vec3 rand_pos = glm::vec3(rand_x, 0.0f, rand_z);
					GLfloat new_height = terrain->find_terrain_height(rand_pos);
					if (new_height > 15.0f && new_height < 100.0f)
					{
						misc_T[i]->update(glm::inverse(misc_T[i]->getTransform()) * glm::translate(glm::mat4(1), glm::vec3(rand_x, (new_height + 6.0f), rand_z)));
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

}


void Window::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (firstMouse)
	{
		lastX = x_pos;
		lastY = y_pos;
		firstMouse = false;
	}

	float xoffset = x_pos - lastX;
	float yoffset = lastY - y_pos;
	lastX = x_pos;
	lastY = y_pos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	if (move_camera) {
		Cam_target = glm::normalize(front); //front is only a direciton
		Cam_Dir = Cam_Pos + Cam_target; // Position + direction gets you the target
		right = glm::normalize(glm::cross(Cam_target, up));
	}

}
void Window::scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	//Opposite of light direction
	glm::vec3 transf = glm::normalize(-lightPos);
	if (mode_one == true || mode_three == true)
	{
		//camera zoom
		if (fov >= 1.0f && fov <= 180.0f)
			fov -= y_offset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 180.0f)
			fov = 180.0f;
	}
	else if (mode_two == true)
	{
		if (y_offset == 1)
		{
			lightPos = lightPos + transf;
		}
		else if (y_offset == -1)
		{
			lightPos = lightPos - transf;
		}

	}
}
glm::vec3 Window::trackBallMapping(glm::vec2 point) //From project 2 
{
	glm::vec3 mousPos;
	float distance;

	mousPos.x = (2.0f * point.x - width) / width;
	mousPos.y = (height - 2.0f * point.y) / height;
	mousPos.z = 0.0f;

	distance = glm::length(mousPos);
	distance = (distance < 1.0f) ? distance : 1.0f;
	mousPos.z = sqrtf(1.001f - distance * distance);

	mousPos = glm::normalize(mousPos);
	return mousPos;
}

void Window::calculate_Frustums()
{
	glm::vec3 normalized_d = glm::normalize(Cam_target);
	glm::vec3 normalized_right = glm::normalize(glm::cross(normalized_d, up));
	glm::vec3 normalizzed_up = glm::cross(normalized_d, normalized_right);
	GLfloat ratio = 1.0f;
	GLfloat nearD = 1.0f;
	GLfloat farD = 250.0f;
	GLfloat tang = glm::tan(glm::radians(frustum_fov) * 0.5);
	GLfloat Hnear = nearD * tang;
	GLfloat Wnear = Hnear * ratio;
	GLfloat Hfar = farD * tang;
	GLfloat Wfar = Hfar * ratio;
	fc = Cam_Pos + (normalized_d * farD);
	ftl = fc + (normalizzed_up * (Hfar)) - (normalized_right * Wfar);
	ftr = fc + (normalizzed_up * (Hfar)) + (normalized_right * Wfar);
	fbl = fc - (normalizzed_up * (Hfar)) - (normalized_right * Wfar);
	fbr = fc - (normalizzed_up * (Hfar)) + (normalized_right * Wfar);

	nc = Cam_Pos + (normalized_d * nearD);
	ntl = nc + (normalizzed_up * Hnear) - (normalized_right * Wnear);
	ntr = nc + (normalizzed_up * Hnear) + (normalized_right * Wnear);
	nbl = nc - (normalizzed_up * Hnear) - (normalized_right * Wnear);
	nbr = nc - (normalizzed_up * Hnear) + (normalized_right * Wnear);


	pl[0] = new Plane(ntr, ntl, ftl); //Top
	pl[1] = new Plane(nbl, nbr, fbr); //Button
	pl[2] = new Plane(ftl, ntl, nbl); //Left
	pl[3] = new Plane(nbr, ntr, fbr); //Right
	pl[4] = new Plane(ntl, ntr, nbr); //Near
	pl[5] = new Plane(ftr, ftl, fbl); //Far
}


void Window::processInput(GLFWwindow* window)
{
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	GLfloat Camera_Speed = Camera_Speed_C * deltaTime;

	if (move_camera == true) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos += Camera_Speed * Cam_target;
				if (Cam_Pos.x < 0.0f)
				{
					Cam_Pos.x = 0;
				}
				else if (Cam_Pos.x > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.x = terrain->getTerrainSize() - 1;
				}
				if (Cam_Pos.z < 0.0f)
				{
					Cam_Pos.z = 0;
				}
				else if (Cam_Pos.z > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.z = terrain->getTerrainSize() - 1;
				}
     //			playermovement -= Camera_Speed * Cam_target;

				if (currentSoundTime < 0)
				{
					soundObject->play2D("audio/grass_walk1.wav", GL_FALSE);
					currentSoundTime = soundInterval;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos -= Camera_Speed * Cam_target;
				if (Cam_Pos.x < 0.0f)
				{
					Cam_Pos.x = 0;
				}
				else if (Cam_Pos.x > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.x = terrain->getTerrainSize()-1;
				}
				if (Cam_Pos.z < 0.0f)
				{
					Cam_Pos.z = 0;
				}
				else if (Cam_Pos.z > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.z = terrain->getTerrainSize()-1;
				}
	//			playermovement += Camera_Speed * Cam_target;

				if (currentSoundTime < 0)
				{
					soundObject->play2D("audio/grass_walk1.wav", GL_FALSE);
					currentSoundTime = soundInterval;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos -= glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
				if (Cam_Pos.x < 0.0f)
				{
					Cam_Pos.x = 0;
				}
				else if (Cam_Pos.x > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.x = terrain->getTerrainSize() - 1;
				}
				if (Cam_Pos.z < 0.0f)
				{
					Cam_Pos.z = 0;
				}
				else if (Cam_Pos.z > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.z = terrain->getTerrainSize() - 1;
				}
//				playermovement += glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
				if (currentSoundTime < 0)
				{
					soundObject->play2D("audio/grass_walk2.wav", GL_FALSE);
					currentSoundTime = soundInterval;
				}
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos += glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
				if (Cam_Pos.x < 0.0f)
				{
					Cam_Pos.x = 0;
				}
				else if (Cam_Pos.x > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.x = terrain->getTerrainSize() - 1;
				}
				if (Cam_Pos.z < 0.0f)
				{
					Cam_Pos.z = 0;
				}
				else if (Cam_Pos.z > terrain->getTerrainSize() - 1)
				{
					Cam_Pos.z = terrain->getTerrainSize() - 1;
				}
//				playermovement -= glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
				if (currentSoundTime < 0)
				{
					soundObject->play2D("audio/grass_walk2.wav", GL_FALSE);
					currentSoundTime = soundInterval;
				}
			}
		}
	}
}

bool Window::return_render(Plane* top, Plane* bottom, Plane* left, Plane* right, Plane* near1, Plane* far1, glm::vec3 center, GLfloat sphere)
{
	//cout << "center : " << center.x << " " << center.y << " " << center.z << endl;
	bool result = true;
	if (glm::dot(center - top->getA(), top->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - top->getA(), top->getNormal()) < sphere)
	{
		result = true;
	}
	if (glm::dot(center - bottom->getA(), bottom->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - bottom->getA(), bottom->getNormal()) < sphere)
	{
		result = true;
	}
	if (glm::dot(center - left->getA(), left->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - left->getA(), left->getNormal()) < sphere)
	{
		result = true;
	}
	if (glm::dot(center - right->getA(), right->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - right->getA(), right->getNormal()) < sphere)
	{
		result = true;
	}
	if (glm::dot(center - near1->getA(), near1->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - near1->getA(), near1->getNormal()) < sphere)
	{
		result = true;
	}
	if (glm::dot(center - far1->getA(), far1->getNormal()) < -sphere)
	{
		result = false;
	}
	else if (glm::dot(center - far1->getA(), far1->getNormal()) < sphere)
	{
		result = true;
	}
	return result;
}