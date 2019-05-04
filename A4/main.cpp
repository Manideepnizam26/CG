#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Scenegraph.h"

#define cout(a) cout<<a<<endl

Model mymodel[4];
View view;
Controller c ;
Scenegraph mygraph;
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
  c.handleCursor( window,xpos,ypos);
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

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	if(glewInit() != GLEW_OK)
	{
		cout("GLEW initialization failed.");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	ifstream plyin1,plyin2,plyin3,plyin4,plyin5;

	plyin3.open("./data/cube.ply");
	plyin4.open("./data/Gallon.ply");
	plyin2.open("./data/sphere.ply");
	plyin1.open("./data/Beethoven.ply");
//	plyin5.open("./data/floor.ply");


	if(!plyin1||!plyin2||!plyin3||!plyin4){
		cout("error in opening plyfiles");
		return 0;
	}
	mymodel[0] = Model(plyin1,0);
	mymodel[1] = Model(plyin2,1);
	mymodel[2] = Model(plyin3,2);
	mymodel[3] = Model(plyin4,3);	
	// mymodel[4] = Model(plyin5,-1);

	view = View();
	view.createShader(vShader,fShader);
	view.createlightShader(lightvshader,lightfshader);
	mygraph = Scenegraph(0,mymodel);
	c = Controller(mymodel,&view,&mygraph);
	mygraph.addNode(0,1);
	mygraph.addNode(1,2);
	mygraph.addNode(2,3);


	while(!glfwWindowShouldClose(mainWindow))
	{

		glfwPollEvents();
		glfwSetKeyCallback(mainWindow,handleKeys );
		glfwSetMouseButtonCallback(mainWindow ,handleMouse);
		glfwSetCursorPosCallback(mainWindow, handleCursor);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		
		mygraph.sceneUpdate();

		view.draw(mymodel[0]);
		view.draw(mymodel[1]);
		view.draw(mymodel[2]);
		view.draw(mymodel[3]);

		glfwSwapBuffers(mainWindow);
	}

	plyin1.close();	plyin2.close();	plyin3.close();	plyin4.close();
	return 0;
}