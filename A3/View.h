#ifndef View_H_
#define View_H_

#include <bits/stdc++.h>
#include "Model.h"
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class View{

private:
    GLuint ShaderID,lightShaderID;
    unsigned int FVAO,FVBO[2];
    float floorVertices[12] = { 1 , -0.1 , -1 , 1  ,-0.1 -1, 0.1 , 0.1 , -1, -0.5, 0.5 ,-1};
    unsigned int floorTriangles[6]= { 0 , 1 , 2 , 0 , 3 , 2 };


public:
    View();
    GLuint getshader();
    GLuint getLightShader();
    void draw(Model &mymodel);
    // void createLightmodel();
    void drawfloor();
    void addShader(const char* shaderIDCode, GLenum shaderIDType, GLuint& shaderid);
    void compileShader(const char* vertexCode, const char* fragmentCode, GLuint& shaderid);
    string readFile(const char* fileLocation);
    void createShader(const char* vertexLocation, const char* fragmentLocation);
    void createlightShader(const char* vertexLocation, const char* fragmentLocation);
};

#endif