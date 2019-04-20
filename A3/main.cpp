#include "Model.h"
#include "View.h"
#include "Controller.h"

#define cout(a) cout<<a<<endl

Model mymodel[5];
View v;
Controller c ;
const char* vShader = "shader.vert";
const char* fShader = "shader.frag";
const char* lightvshader = "lightvshader.vert";
const char* lightfshader = "lightfshader.frag";

static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode){
  c.handleKeys(window,key,code,action,mode);
}

static void handleMouse(GLFWwindow* window, int button , int action, int mode){
  c.handleMouse(window,button,action,mode);
}

static void handleCursor(GLFWwindow* window,double xpos, double ypos){
  c.handleCursor( window);
}


int main(void) {

	const GLint WIDTH = 800, HEIGHT = 600;

	if(!glfwInit())
	{
		cout("GLFW initialization failed.");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 3", NULL, NULL);
	if(!mainWindow)
	{
		cout("GLFW window creation failed.");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		cout("GLEW initialization failed.");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	ifstream plyin1,plyin2,plyin3,plyin4,plyin5;

	plyin1.open("cube.ply");
	plyin2.open("ketchup.ply");
	plyin3.open("sphere.ply");
	plyin4.open("Beethoven.ply");
	plyin5.open("floor.ply");


	if(!plyin1||!plyin2||!plyin3||!plyin4||!plyin5){
		cout("error in opening plyfiles");
		return 0;
	}
	mymodel[0] = Model(plyin1,0);
	mymodel[1] = Model(plyin2,1);
	mymodel[2] = Model(plyin3,2);
	mymodel[3] = Model(plyin4,3);	
	// mymodel[4] = Model(plyin5,-1);

	v = View();
	v.createShader(vShader,fShader);
	v.createlightShader(lightvshader,lightfshader);
	c = Controller(mymodel);

	while(!glfwWindowShouldClose(mainWindow))
	{

		glfwPollEvents();
		glfwSetKeyCallback(mainWindow,handleKeys );
		glfwSetMouseButtonCallback(mainWindow ,handleMouse);
		glfwSetCursorPosCallback(mainWindow, handleCursor);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		v.draw(mymodel[0]);
		v.draw(mymodel[1]);
		v.draw(mymodel[2]);
		v.draw(mymodel[3]);
		v.draw(mymodel[4]);

		glfwSwapBuffers(mainWindow);
	}

	plyin1.close();	plyin2.close();	plyin3.close();	plyin4.close();
	return 0;
}