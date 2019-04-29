#include "Scenegraph.h"
using namespace std;

Scenegraph::Scenegraph(){ Root = NULL;}

Scenegraph::Scenegraph(int rootID,Model* mymodel){
    Root = createNode(rootID);
    models = mymodel;
}

Scenegraph::Node* Scenegraph::createNode(int nodeID){
    Node* newNode = new Node;
    newNode->ID = nodeID;
    return newNode;
}

Scenegraph::Node *Scenegraph::getNode(int nodeID){

    if(Root == NULL){ return NULL;}

    std::queue<Node*> nodes;
    int size=0;
    nodes.push(Root);
    size++;

    while(size>0){
        Node* first = nodes.front();
        nodes.pop();
        size--;
        if(first->ID == nodeID){ return first; }

        for(int i=0;i<first->children.size(); i++){
            nodes.push(first->children[i]);
            size++;
        }
    }
    return NULL;
}

void Scenegraph::addNode(int parentID, int newID){
    Node* parent = getNode(parentID);
    Node* newNode = createNode(newID);
    parent->children.push_back(newNode);
}

// void Scenegraph::rotate(int parentID, int childID){
//     float x = models[childID].get_x() - models[parentID].get_x();
//     float z = models[childID].get_z() - models[parentID].get_z();

//     float theta;

//     theta = atan(z/x);

//     x = models[parentID].get_x() + (1.0*sin(theta+speed ));
    
// }


void Scenegraph::updateModel(int parentID,int modelID , int level){

    if(level==1){
        float x = models[parentID].get_x() + (1.0)*cos(angle); 
        float y = models[parentID].get_y() ;
        float z = models[parentID].get_z() + (1.0)*sin(angle); 


        models[modelID].set_x(x);
        models[modelID].set_y(y);
        models[modelID].set_z(z);
        angle +=0.01;
    }
}

void Scenegraph::sceneUpdate(){

    std::queue<Node*> nodes;
    int size=0;
    int level=1;
    nodes.push(Root);

    while(nodes.size()!=0){
        size = nodes.size();
        while(size!=0){
            Node* first = nodes.front();
            nodes.pop();
            // cout<<first->ID<<endl;
            size--;
            for(int i=0;i<first->children.size(); i++){
                nodes.push(first->children[i]);
                updateModel(first->ID , first->children[i]->ID , level);
            }
        }
        level++;
    }
}