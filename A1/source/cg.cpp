#include<bits/stdc++.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
using namespace std;

#define cout(a) cout<<a<<endl
float scale = 1.0, x = 0.0, y = 0.0, theta=0;
GLfloat width = 0.328f;
const GLint WIDTH = 600,HEIGHT = 600;
bool leftStatus = false;
double xpos, ypos , xfinal , yfinal, zfinal , tempx , tempy;
GLdouble modelview[16],projection[16];
GLint viewport[4];
int pos,rightStatus=-1,angle=200;

string Buttons[19] = {"0",".","=","1","2","3","+","4","5","6","-","7","8","9","*","AC","+/-","%","/"};
GLfloat Left[19]   = {-1.0f,-0.34f,0.0f,-1.0f,-0.67f,-0.34f,0.0f,-1.0f,-0.67f,-0.34f,0.0f,-1.0f,-0.67f,-0.34f,0.0f,-1.0f,-0.67f,-0.34f,0.0f};
GLfloat Bottom[19] = {-1.0f,-1.0f,-1.0f,-0.67f,-0.67f,-0.67f,-0.67f,-0.34f,-0.34f,-0.34f,-0.34f,-0.01f,-0.01f,-0.01f,-0.01f,0.32f,0.32f,0.32f,0.32f};


class Point{

    public:

    GLfloat _x,_y,_z;

    Point(){ }
    Point(GLfloat x , GLfloat y, GLfloat z){
        _x = x;
        _y = y;
        _z = z;
    }

    GLfloat getX(){
        return _x;
    }

    GLfloat getY(){
        return _y;
    }

    GLfloat getZ(){
        return _z;
    }

    void setX(GLfloat x){ _x = x; }
    void setY(GLfloat y){ _y = y; }
    void setZ(GLfloat z){ _z = z; }

};

class Rectangle{

    public:

    Point* _BottomLeft;
    GLfloat _length , _breadth;


    Rectangle(Point* BottomLeft , GLfloat length , GLfloat breadth){
        _BottomLeft = BottomLeft;
        _length = length;
        _breadth = breadth;
    }

    Point* getBottomLeft(){ return _BottomLeft; }
    GLfloat getLength(){   return _length; }
    GLfloat getBreadth(){  return _breadth; }
    
    void setPoint(Point* poi){ 
        _BottomLeft->setX(poi->getX());
        _BottomLeft->setY(poi->getY());
    }

    bool check(Point p){
        if(p.getX() >= _BottomLeft->getX() && p.getX() <= _BottomLeft->getX() + _length && 
           p.getY() >= _BottomLeft->getY() && p.getY() <= _BottomLeft->getY() + _breadth){
               return true;
        }
        return false;
    }

    void drawRectangle(int t){
        glLoadIdentity();
        glTranslatef(x,y,0);
        glRotatef(theta ,0.0f,0.0f,1.0f);
    	glScalef(scale,scale,scale);

    	GLfloat xmid,ymid;
	    xmid = _BottomLeft->getX()+_length/2;
	    ymid = _BottomLeft->getY()+ _breadth/2;

    	glTranslatef(xmid,ymid,0);
	    glRotatef(t,0.0f,0.0f,1.0f);

	    glBegin(GL_LINES);
    	glColor4f(1.0f,0.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
	    glVertex3f(1.0f,0.0f,0.0f);
    	glColor4f(0.0f,1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
	    glVertex3f(0.0f,-1.0f,0.0f);
	    glEnd();

    	glBegin(GL_LINE_LOOP);
	    glColor4f(1,0,0,1);

		glVertex3f(_length/2,_breadth/2,0);
    	glVertex3f(-_length/2,_breadth/2,0);
    	glVertex3f(-_length/2,-_breadth/2,0);
	    glVertex3f(_length/2,-_breadth/2,0);
	
    	glEnd();

    }
};

class Calculator{

    public:

    Rectangle* _vertex[19];

    Calculator(){

        Point* p = new Point( Left[0] , Bottom[0] , 0.0f);
        _vertex[0] = new Rectangle(p,2*width,width);

        for(int i=1;i<19;i++){
            Point* p = new Point( Left[i] , Bottom[i] , 0.0f);
            _vertex[i] = new Rectangle(p,width,width);
        }        
    }

    Rectangle* getVertex(int p){ return _vertex[p];}

    void setVertex(int p, Point* poi){
        _vertex[p]->setPoint(poi);
        // cout<<poi.getX()<<endl;
    }

    int findpos(Point p){
        for(int i=0;i<19;i++){
            if(_vertex[i]->check(p)){
                // cout(Buttocout(Buttons[i]);
                return i;
            }
        }
        return -1;
    }

    void drawCalculator(){
        glLoadIdentity();
        glTranslatef(x,y,0);
        glRotatef(theta,0.0f,0.0f,1.0f);
	    glScalef(scale,scale,scale);

	    glBegin(GL_LINES);
    	glColor4f(1.0f,0.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
	    glVertex3f(1.0f,0.0f,0.0f);
    	glColor4f(0.0f,1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
	    glVertex3f(0.0f,-1.0f,0.0f);
	    glEnd();


        glBegin(GL_QUADS);
        for(int i=0;i<19;i++){
		    glColor4f(0.65f, 0.65f, 0.65f, 1.0f);
		    if(i%4==2){
			    glColor4f(1.0f, 0.25f, 0.0f, 1.0f);
			}

            glVertex3f(_vertex[i]->getBottomLeft()->getX(),_vertex[i]->getBottomLeft()->getY(),0.0f);
            glVertex3f(_vertex[i]->getBottomLeft()->getX() + _vertex[i]->getLength(),_vertex[i]->getBottomLeft()->getY(),0.0f);
            glVertex3f(_vertex[i]->getBottomLeft()->getX() + _vertex[i]->getLength(),_vertex[i]->getBottomLeft()->getY()+ _vertex[i]->getBreadth(),0.0f);
            glVertex3f(_vertex[i]->getBottomLeft()->getX(),_vertex[i]->getBottomLeft()->getY() + _vertex[i]->getBreadth(),0.0f);            
        }
        glEnd();
    }

};

class HandleEvents{
    public:
    HandleEvents(){ }

    static void UpdatePosition(){

		ypos = HEIGHT-ypos;
		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glGetIntegerv(GL_VIEWPORT,viewport);		
		gluUnProject(xpos,ypos,0.0,modelview,projection,viewport,&xfinal,&yfinal,&zfinal);

    }

    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mods)
    {
	    if(key == GLFW_KEY_R && action == GLFW_PRESS)
    	{
	    	theta+=45;
	    }
    	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	    {
		    glfwSetWindowShouldClose(window, GL_TRUE);
	    }
	    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	    {
		    scale += 0.05;
    	}
	    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	    {
		    scale -= 0.05;
	    }
	    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	    {
		    x -= 0.05;
	    }
	    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	    {
		    x += 0.05;
    	}
	    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	    {
		    y += 0.05;
	    }
	    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	    {
		    y -= 0.05;
	    }	
    }

    static void handleMouse(GLFWwindow* window, int button, int action, int mods){
        

        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            glfwGetCursorPos(window, &xpos, &ypos);
            UpdatePosition();
            leftStatus = true;
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
            glfwGetCursorPos(window, &xpos, &ypos);
            UpdatePosition();
            rightStatus = 0;
            // cout("CLICKED");
        }

        if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
            glfwGetCursorPos(window, &xpos, &ypos);
            UpdatePosition();
            rightStatus = 1;
            // cout("RELEASED");
        }
    }

};


int main(void){

    if(!glfwInit())
    {
		cout("GLFW initialization failed.");
		glfwTerminate();
		return 1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	// int bufferWidth, bufferHeight;
	// glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	// glewExperimental = GL_TRUE;

    Calculator* calculator = new Calculator();
    HandleEvents* handlevents = new HandleEvents();

    Rectangle* rectangle;
    Point* point;


	while(!glfwWindowShouldClose(mainWindow)){

		glfwPollEvents();
		glfwSetKeyCallback(mainWindow, handlevents->handleKeys);
		glfwSetMouseButtonCallback(mainWindow, handlevents->handleMouse);


		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		calculator->drawCalculator();

        if(rightStatus==0){
            point = new Point(xfinal,yfinal,0);
            pos = calculator->findpos(*point);
            if(pos!=-1){
                tempx = xfinal - calculator->getVertex(pos)->getBottomLeft()->getX();
                tempy = yfinal - calculator->getVertex(pos)->getBottomLeft()->getY();
            }
        }

        else if(rightStatus ==1){
            point = new Point(xfinal - tempx ,yfinal - tempy,0);

            if(pos!=-1){
                calculator->setVertex(pos,point);
            }
            rightStatus = -1;
        }

        if(leftStatus==true){
            point = new Point(xfinal,yfinal,0);
            pos = calculator->findpos(*point);
            leftStatus = false;
            if(pos!=-1){
                rectangle = calculator->getVertex(pos);
                cout(Buttons[pos]);
                angle = 0;
            }
        }

        if(angle<=180){
            rectangle->drawRectangle(angle);
            angle+=5;
        }

		glfwSwapBuffers(mainWindow);
	}

    return 0;
}