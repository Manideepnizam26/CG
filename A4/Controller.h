#ifndef Controller_H_
#define Controller_H_

#include "Model.h"
#include "Scenegraph.h"
#include "Trackball.h"
#include "View.h"

#include<iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Controller{
    private:
    float x,y,z,scale;
	int modelId;
	bool isHold;
	Model* model;
	Scenegraph* graph;
	View* view;
	double tempX,tempY,tempZ,finalX,finalY,finalZ;
	double prevX,prevY,currX,currY;
	GLdouble modelview[16],projection[16];
	GLint viewport[4];
	float q[4];
	glm::mat4 rotMatrix;
	float yaw   = -90.0f, pitch =  0.0f, lastX =  800.0f / 2.0, lastY =  600.0 / 2.0;
	bool firstMouse = true;


	public:
		Controller();
		Controller(Model* mymodel,View* myview, Scenegraph* mygraph);
		void UpdateModel();
		void setModelId(int id);
		float getX();
		float getY();
		float getZ();
		float getScale();
		float getSpeed();
		void setSpeed(float s);
		void setMatrix(glm::mat4 matrix);
		void UpdatePosition();
		glm::mat4 getMatrix();
	    void handleKeys(GLFWwindow* window, int key, int code, int action, int mods);
	    void handleMouse(GLFWwindow* window, int button, int action, int mods);
		void handleCursor(GLFWwindow* window , double xpos, double ypos);
		void changeMapping();
		void changeTexture();

};

#endif