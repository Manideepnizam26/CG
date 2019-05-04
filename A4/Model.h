#ifndef Model_H_
#define Model_H_

#include "stb_image.h"
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>


#include <bits/stdc++.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

class Model{

private:
unsigned int VAO,VBO[6],Texture[4],LVAO,LVBO;
int numberOfVertices,numberOfTriangles,mappingId=0,textureId=0;
glm::vec3 lightPosition;
bool toRotate;
float angle;
float *Vertices;
float *Normals;
float *SphericalTexels;
float *cylindricalTexels;
float *OpenGlTexels;
glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 templightColor = lightColor;


unsigned int *Triangles;
float scale,x,y,z,lightx,lighty,lightz;
glm::mat4 rotationMatrix;
glm::mat4 modelMatrix;

public:

Model();
Model(ifstream& plyfile,int i);
  float get_x();
  float get_y();
  float get_z();
  float getAngle();
  float get_scale();
  void changeMapping();
  void changeTexture();
  void changeRotate();
  void set_scale(float s);
  void set_x(float a);
  void set_y(float b);
  void set_z(float c);
  void setAngle(float a);
  glm::mat4 getMatrix();
  void setrotationMatrix(glm::mat4 mat);
  void setmodelMatrix(glm::mat4 mat);
  unsigned int get_vao();
  unsigned int get_lvao();
  unsigned int* get_vbo();
  int getNoVertices();
  int getNoTriangles();
  void initModel();
  void updateangle();
  void normalize();
  void computeNormals();
  void generateSpherical();
  void generateCylindrical();
  void generateOpengl();
  void createTexture(std::string filename, unsigned int &textureID);
  void createLightmodel(int index);
  glm::vec3 getLightColor();
  void setLightColor(glm::vec3 color);
  void toggleLight();
  glm::vec3 getLightPos();
};

#endif