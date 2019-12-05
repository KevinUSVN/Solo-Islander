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
glm::vec3 Window::lightPos;
Geometry* testing_ground;

/*
	Initialize all the Transformation here
*/
glm::vec3 playermovement = glm::vec3(0.0f, 0.0f, 0.0f);
Transform* Window::world_T_matrix;
Transform* bunnyT;
Transform* player_T;
Transform* player_right_hand_T;
Transform* player_left_hand_T;
vector<Transform*> player_bounding_box_T(4);
Transform* testing_ground_T;
Transform* world_physic_T;

/*
	Initialize all the scene object here
*/
Geometry* light_object;


/*
	Initialize all the player object here
*/
vector<Geometry*> player_bounding_box(4);
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

/*
	Initialize all the time variable here
*/
float last_time = 0.0f;
float current_time = 0.0f;



/*
	Initialize all the camera variable here
*/
glm::vec3 Window::last_mouse_pos;
glm::vec3 Window::current_mouse_pos;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::Cam_target(0, 0, -1);
glm::vec3 Window::Cam_Pos(0, 0, 2.0f); // Camera position.
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
	Initialize all shader programs in this function.
*/
bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	default_program = LoadShaders("../shaders/shader.vert", "../shaders/shader.frag");
	normal_color_program = LoadShaders("../shaders/shader_normal.vert", "../shaders/shader_normal.frag");
	skybox_program = LoadShaders("../shaders/skybox.vert", "../shaders/skybox.frag");
	texture_program = LoadShaders("../shaders/cubemaps.vert", "../shaders/cubemaps.frag");
	line_program = LoadShaders("../shaders/line_shader.vert", "../shaders/line_shader.frag");
	point_program = LoadShaders("../shaders/point_shader.vert", "../shaders/point_shader.frag");
	handle_program = LoadShaders("../shaders/handle_shader.vert", "../shaders/handle_shader.frag");
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
	program = default_program;
	// Activate the shader program.
	glUseProgram(program);
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
	testing_ground = new Geometry("../OBJ_files/testing_ground.obj",1);
	testing_ground->scale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
	//Scenic Object

	//Player Object
	player_bounding_box[PLAYER::body] = new Geometry("../OBJ_files/square.obj",1);
	player_bounding_box[PLAYER::left_hand] = new Geometry("../OBJ_files/square.obj", 1);
	player_bounding_box[PLAYER::right_hand] = new Geometry("../OBJ_files/square.obj", 1);
	player_bounding_box[PLAYER::head] = new Geometry("../OBJ_files/square.obj", 1);
	player = new Geometry("../OBJ_files/square.obj",1);
	Player_right_hand = new Geometry("../OBJ_files/RiggedHandRight.obj",1);
	Player_left_hand = new Geometry("../OBJ_files/RiggedHandLeft.obj",1);


	return true;
}
/*
	Initialize all transformation in this function.
*/
bool Window::initializeTransforms() {
	//global transform
	world_T_matrix = new Transform(glm::mat4(1));
	testing_ground_T = new Transform(glm::translate(glm::vec3(-500.0, -20.0f, -500.0f)));
	world_physic_T = new Transform(glm::mat4(1));
	//Initialize physic

	//Object transform


	//Player Transform
	player_T = new Transform(glm::mat4(1));
	player_right_hand_T = new Transform(glm::translate(glm::mat4(1),glm::vec3(-1.0f, 0.0f, -2.0f)));
	player_left_hand_T = new Transform(glm::translate(glm::mat4(1),glm::vec3(1.0f, 0.0f, -2.0f)));
	player_bounding_box_T[PLAYER::body] = new Transform(glm::mat4(1));
	player_bounding_box_T[PLAYER::left_hand] = new Transform(glm::translate(glm::vec3(-2.0, 0.0f, -2.0f)));
	player_bounding_box_T[PLAYER::right_hand] = new Transform(glm::translate(glm::vec3(2.0, 0.0f, -2.0f)));
	player_bounding_box_T[PLAYER::head] = new Transform(glm::translate(glm::vec3(0, 4.0f, 0.0f)));
	//Environment transform
	currentNode = world_T_matrix;
	return true;
}
/*
	Apply any transform tree structure here.
*/
bool Window::applyTransforms() {
	//Global
	world_physic_T->addChild(player_T);
	world_T_matrix->addChild(testing_ground_T);

	//Environmental
	testing_ground_T->addChild(testing_ground);

	//Scenic

	//Player
	player_T->addChild(player);
	player_T->addChild(player_right_hand_T);
	player_T->addChild(player_left_hand_T);
	player_T->addChild(player_bounding_box_T[PLAYER::body]);
	player_T->addChild(player_bounding_box_T[PLAYER::left_hand]);
	player_T->addChild(player_bounding_box_T[PLAYER::right_hand]);
	player_T->addChild(player_bounding_box_T[PLAYER::head]);
	player_right_hand_T->addChild(Player_right_hand);
	player_left_hand_T->addChild(Player_left_hand);
	player_bounding_box_T[PLAYER::body]->addChild(player_bounding_box[PLAYER::body]);
	player_bounding_box_T[PLAYER::left_hand]->addChild(player_bounding_box[PLAYER::left_hand]);
	player_bounding_box_T[PLAYER::right_hand]->addChild(player_bounding_box[PLAYER::right_hand]);
	player_bounding_box_T[PLAYER::head]->addChild(player_bounding_box[PLAYER::head]);
	player_bounding_box_T[PLAYER::body]->set_render(false);
	player_bounding_box_T[PLAYER::left_hand]->set_render(false);
	player_bounding_box_T[PLAYER::right_hand]->set_render(false);
	player_bounding_box_T[PLAYER::head]->set_render(false);
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

}

void Window::displayCallback(GLFWwindow* window)
{
	GLfloat new_time =  glfwGetTime();
	Cam_Pos = glm::vec3(Cam_Pos.x,gravity->calculate_new_Pos(new_time).y, Cam_Pos.z);
	projection = glm::perspective(glm::radians(fov), double(width) / (double)height, 1.0, 1400.0);
	view = glm::lookAt(Window::Cam_Pos, Window::Cam_Pos + Window::Cam_target, Window::up);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Draw The sphere
	glUseProgram(texture_program);
	glUniform1i(glGetUniformLocation(texture_program, "skybox"), 0);
	currentNode->draw(glm::mat4(1), texture_program);
	glUniformMatrix4fv(glGetUniformLocation(texture_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(texture_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(texture_program, "cameraPos"), Cam_Pos.x, Cam_Pos.y, Cam_Pos.z);
	//Draw the skybox
	player_T->draw(glm::inverse(view), texture_program);

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
			if (normal_on)
			{
				program = normal_color_program;
				normal_on = !normal_on;
			}
			else
			{
				program = default_program;
				normal_on = !normal_on;
			}
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
     //			playermovement -= Camera_Speed * Cam_target;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos -= Camera_Speed * Cam_target;
	//			playermovement += Camera_Speed * Cam_target;

			}
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos -= glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
//				playermovement += glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (move_camera) {
				Prev_Cam_Pos = playermovement;
				Cam_Pos += glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
//				playermovement -= glm::normalize(glm::cross(Cam_target, up)) * Camera_Speed;
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

