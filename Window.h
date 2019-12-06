#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "shader.h"
#include "Transform.h"
#include "Node.h"
#include "Geometry.h"
#include "Node.h"
#include "physic.h"
#include "Plane.h"
#include "Terrain.h"
#include "Skybox.h"
#include <GL/glut.h>

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;

	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 Cam_Pos, Cam_Dir, up, Cam_target, right, Prev_Cam_Pos;

	static glm::vec3 last_mouse_pos;
	static glm::vec3 current_mouse_pos;
	static GLuint program, default_program, normal_color_program, skybox_program, texture_program,
		line_program, point_program, handle_program, terrain_program;

	static Transform* world_T_matrix;
	static Geometry* obj;
	static Node* currentNode;
	static Skybox* skybox;
	static Plane* plane;




	static glm::vec3 lightPos;
	static std::vector<Node*> deleteTracker;

	static bool initializeProgram();
	static bool initializeObjects();
	static bool initializeTransforms();
	static bool applyTransforms();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
	static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
	static glm::vec3 trackBallMapping(glm::vec2 point);
	static void calculate_Frustums();
	static void processInput(GLFWwindow* window);
	static bool return_render(Plane* top, Plane* bottom, Plane* left, Plane* right, Plane* near, Plane* far, glm::vec3 center, GLfloat sphere);


};

#endif
