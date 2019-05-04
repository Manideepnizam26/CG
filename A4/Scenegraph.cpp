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

void Scenegraph::rotate(int parentID, int modelID){

    float x1 = models[parentID].get_x();
    float z1 = models[parentID].get_z();
    float x2 = models[modelID].get_x();
    float z2 = models[modelID].get_z();

    float radius = sqrt( (z2-z1)*(z2-z1) + (x2-x1)*(x2-x1) );

    float x = models[parentID].get_x() + sin(angle); 
    float y = models[parentID].get_y();
    float z = models[parentID].get_z() + cos(angle); 

    x+=  (0.1)*sin(10*angle);
    models[modelID].set_x(x);
    models[modelID].set_y(y);
    models[modelID].set_z(z);
    angle +=speed;
}

void Scenegraph::moveTowards(int parentID, int modelID){

    float x1 = models[parentID].get_x();
    float y1 = models[parentID].get_y();
    float z1 = models[parentID].get_z();

    float x2 = models[modelID].get_x();
    float y2 = models[modelID].get_y();
    float z2 = models[modelID].get_z();

    float d1 = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1) );
    float temp = speed/2*d1;

    float x = x2 + temp*(x1-x2);
    float y = y2 + temp*(y1-y2);
    float z = z2 + temp*(z1-z2);

    models[modelID].set_x(x);
    models[modelID].set_y(y);
    models[modelID].set_z(z);
}

void Scenegraph::jump(int parentID, int modelID){

    models[modelID].set_x( models[parentID].get_x() );
    models[modelID].set_z( models[parentID].get_z() );

    float y = models[parentID].get_y()+ 0.35;

    jump_factor+=speed;
    y+=(0.2)*(1+sin(10*jump_factor));

    models[modelID].set_y(y);    
}

void Scenegraph::updateModel(int parentID,int modelID , int level){

    if(level==1){        
        rotate(parentID,modelID);
    }

    else if(level==2){
        moveTowards(parentID,modelID);
    }

    else if(level==3){
        jump(parentID,modelID);
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
            models[first->ID].updateangle();
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