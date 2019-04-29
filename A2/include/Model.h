#ifndef Model_H_
#define Model_H_

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
unsigned int VAO,VBO[3],LVAO,LVBO,CVAO,CVBO[3];
int numberOfVertices,numberOfTriangles,numberOfSegments=15;
glm::vec3 lightPosition;
float *Vertices;
float *Normals;
float *TriangleNormals;
float *InCenters;
float *InRadius;
float *CircleVertices;
float *SplatVertices;
float *SplatColor;
glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 templightColor = lightColor;
int renderId=0;


unsigned int *Triangles;
unsigned int *SplatTriangles;
unsigned int *CircleTriangles;
float scale,x,y,z,lightx,lighty,lightz;
glm::mat4 rotationMatrix;

public:

Model();
Model(ifstream& plyfile,int i);
  float get_x();
  float get_y();
  float get_z();
  float get_scale();
  void set_scale(float s);
  void set_x(float a);
  void set_y(float b);
  void set_z(float c);
  glm::mat4 getMatrix();
  void setMatrix(glm::mat4 mat);
  unsigned int get_vao();
  unsigned int get_lvao();
  unsigned int get_cvao();
  unsigned int* get_vbo();
  int getNoVertices();
  int getNoTriangles();
  int getNoSegments();
  void initModel(int index);
  void normalize();
  void computeTriangleNormals();
  void computeNormals();
  void createLightmodel(int index);
  glm::vec3 getLightColor();
  void setLightColor(glm::vec3 color);
  void toggleLight();
  glm::vec3 getLightPos();
  void printInfo();
  void computeIncenters();
  void computeInradius();
  void  generateCircleVertices();
  void  generateCircleTriangles();
  void  generateSplatVertices();
  void  genetateSplatTriangles();
  void  generateSplatColors();
  float getAngle(int index);
  glm::vec3 getAxis(int index);
  void changeRendering();
  int getRenderId();
};

#endif