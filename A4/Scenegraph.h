#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_


#include "Model.h"
#include <bits/stdc++.h>
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

class Scenegraph{

private:

    struct Node{
        int ID;
        vector <Node*> children; 
    };
    Node* Root;
    Model* models;
    float angle=0;
    float jump_factor = 0.01;
    float speed = 0.01;

public:
    Scenegraph();
    Scenegraph(int rootId,Model* mymodel);
    Node *createNode(int nodeID);
    Node *getNode(int nodeID);
    void addNode(int parentID, int newID);
    void sceneUpdate();
    void updateModel(int parentID, int modelID , int level);
    void rotate(int parentID, int modelID);
    void moveTowards(int parentID, int modelID);
    void jump(int parentID, int modelID);
    float getSpeed(){ return speed; }
    void setSpeed(float s){ speed = s; }
};

#endif
