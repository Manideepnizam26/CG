#include "Controller.h"
// #include "Model.h"
using namespace std;


Controller::Controller(){
		x=0;
		y=0;
    z=0;
		scale=1.0;
		isHold = false;
}

Controller::Controller(Model* mymodel){
		model = mymodel;
		isHold = false;
		setModelId(0);
}

void Controller::setModelId(int id){
		modelId=id;
		x = model[modelId].get_x();
		y = model[modelId].get_y();
		z = model[modelId].get_z();
		scale = model[modelId].get_scale();
		rotMatrix = model[modelId].getMatrix();
}


void Controller::setMatrix(glm::mat4 matrix){
		rotMatrix=matrix;
}

void Controller::UpdateModel(){
		model[modelId].set_x(x);
		model[modelId].set_y(y);
		model[modelId].set_z(z);
		model[modelId].set_scale(scale);
		model[modelId].setMatrix(rotMatrix);
}

float Controller::getX(){ return x; }
float Controller::getY(){ return y; }
float Controller::getZ(){ return z; }
float Controller::getScale(){ return scale; }
glm::mat4 Controller::getMatrix(){ return rotMatrix;}	


void Controller::UpdatePosition(){
		tempY = 600-tempY;
		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,viewport);		
		gluUnProject(tempX,tempY,0.0,modelview,projection,viewport,&finalX,&finalY,&finalZ);
}


void Controller::changeMapping(){
			model[0].changeMapping();
			model[1].changeMapping();
			model[2].changeMapping();
			model[3].changeMapping();
}

void Controller::changeTexture(){
			model[0].changeTexture();
			model[1].changeTexture();
			model[2].changeTexture();
			model[3].changeTexture();
}



void Controller::handleKeys(GLFWwindow* window, int key, int code, int action, int mods){

		if (key ==GLFW_KEY_1  && action == GLFW_PRESS){
				setModelId(0);
	  }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS){
				setModelId(1);
	  }
  	if (key == GLFW_KEY_3 && action == GLFW_PRESS){
				setModelId(2);
	  }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS){
				setModelId(3);
	  }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		    glfwSetWindowShouldClose(window, GL_TRUE);
	  }
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
		    scale+= 0.05;
				UpdateModel();
  	}
	  if (key == GLFW_KEY_MINUS && action == GLFW_PRESS){
		    scale -= 0.05;
				UpdateModel();
	  }
	  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		    x -= 0.05;
				UpdateModel();
	  }
	
	  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){			
			  x += 0.05;
				UpdateModel();			
    }
	  if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		    y += 0.05;
				UpdateModel();
    }
	  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		    y -= 0.05;
				UpdateModel();
    }

		if(key ==GLFW_KEY_M && action == GLFW_PRESS){
			changeMapping();
		}

		if(key ==GLFW_KEY_T && action == GLFW_PRESS){
			changeTexture();
		}
		if(key ==GLFW_KEY_L && action == GLFW_PRESS){
			model[modelId].toggleLight();
		}
}

void Controller::handleMouse(GLFWwindow* window, int button, int action, int mods){


    if((button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)){
        glfwGetCursorPos(window, &tempX, &tempY);
				UpdatePosition();
				prevX = finalX;
				prevY = finalY;
				isHold = true;
    }

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
				glm::mat4 temp1,temp2;
        glfwGetCursorPos(window, &tempX, &tempY);
				UpdatePosition();
				currX = finalX;
				currY = finalY;
				trackball(q,prevX,prevY,currX,currY);
				temp1 = rotMatrix;
				temp2 = build_rotmatrix(q);
				rotMatrix = temp1*temp2;
				UpdateModel();
				isHold = false;
	  }
}

void Controller::handleCursor(GLFWwindow* window ){

		if(isHold){

				glm::mat4 temp1,temp2;
        glfwGetCursorPos(window, &tempX, &tempY);
				UpdatePosition();
				currX = finalX;
				currY = finalY;
				trackball(q,prevX,prevY,currX,currY);
				temp1 = rotMatrix;
				temp2 = build_rotmatrix(q);
				rotMatrix = temp1*temp2;
				UpdateModel();

        glfwGetCursorPos(window, &tempX, &tempY);
				UpdatePosition();
				prevX = currX;
				prevY = currY;
		}
}