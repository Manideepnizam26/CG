#include "../include/Model.h"
using namespace  std;

Model::Model(){}

Model::Model(ifstream& plyfile,int index){

    scale = 0.2;
    x=-0.75+0.5*index;
    y=0.0;
    z=0.0;

    rotationMatrix=glm::mat4(1);
    string notUsedInfo;
    float tempFloat;

    plyfile>>notUsedInfo;
    plyfile>>notUsedInfo>>notUsedInfo>>tempFloat;
    plyfile>>notUsedInfo>>notUsedInfo>>numberOfVertices;
    plyfile>>notUsedInfo>>notUsedInfo>>notUsedInfo;
    plyfile>>notUsedInfo>>notUsedInfo>>notUsedInfo;
    plyfile>>notUsedInfo>>notUsedInfo>>notUsedInfo;
    plyfile>>notUsedInfo>>notUsedInfo>>numberOfTriangles;
    plyfile>>notUsedInfo>>notUsedInfo>>notUsedInfo>>notUsedInfo>>notUsedInfo;
    plyfile>>notUsedInfo;

    Vertices = new float[3*numberOfVertices];
    Normals = new float[3*numberOfVertices];
    Triangles = new unsigned int[3*numberOfTriangles];
    TriangleNormals = new float[3*numberOfTriangles];
    InCenters = new float[3*numberOfTriangles];
    InRadius = new float[numberOfTriangles];
    CircleVertices = new float[3*(numberOfSegments+1)];
    CircleTriangles = new unsigned int[3*numberOfSegments];
    SplatVertices = new float[3*numberOfTriangles*(numberOfSegments+1)];
    SplatTriangles = new unsigned int[3*numberOfTriangles*numberOfSegments];
    SplatColor = new float[3*numberOfTriangles*(numberOfSegments+1)];

    for(int i=0;i<3*numberOfVertices;i++){
        plyfile>>tempFloat;
        Vertices[i] = tempFloat;
    }

    for(int i=0;i<numberOfTriangles;i++){
        int no,p,q,r;
        plyfile>>no;
        plyfile>>p>>q>>r;
        Triangles[3*i]=p;
        Triangles[3*i+1]=q;
        Triangles[3*i+2]=r;
    }

    lightPosition.x = -0.75f+0.5f*index;
    lightPosition.y = 0.75f;
    lightPosition.z = 0.0f;

    initModel(index);
}

void Model::initModel(int index){

    normalize();
    computeTriangleNormals();
    computeNormals();
    computeInradius();
    computeIncenters();
    createLightmodel(index);
    generateCircleVertices();
    generateCircleTriangles();
    generateSplatVertices();
    genetateSplatTriangles();
    generateSplatColors();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[3*numberOfVertices]), Vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int[3*numberOfTriangles]), Triangles, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[3*numberOfVertices]), Normals,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &CVAO);
    glBindVertexArray(CVAO);
    glGenBuffers(3, CVBO);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[3*numberOfTriangles*(numberOfSegments+1)]), SplatVertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int[3*numberOfTriangles*numberOfSegments]), SplatTriangles, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, CVBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[3*numberOfTriangles*(numberOfSegments+1)]) , SplatColor ,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    templightColor = lightColor;

}

void Model::normalize(){

    float maxX,minX,maxY,minY,maxZ,minZ;
    maxY = maxZ = maxX = -100000;
    minY = minZ = minX = 100000;

    for(int i=0;i<numberOfVertices;i++){
        maxX = max(maxX,Vertices[3*i]);
        maxY = max(maxY,Vertices[3*i+1]);
        maxZ = max(maxZ,Vertices[3*i+2]);

        minX = min(minX,Vertices[3*i]);
        minY = min(minY,Vertices[3*i+1]);
        minZ = min(minZ,Vertices[3*i+2]);
    }

    float compressFactor = 2.0/max(maxX-minX,max(maxY-minY,maxZ-minZ));

    for(int i=0;i<numberOfVertices;i++){

        Vertices[3*i] -= (maxX+minX)/2;
        Vertices[3*i+1] -= (maxY+minY)/2;
        Vertices[3*i+2] -= (maxZ+minZ)/2;
        Vertices[3*i]*=compressFactor;
        Vertices[3*i+1]*=compressFactor;
        Vertices[3*i+2]*=compressFactor;
    }
}

void Model::computeTriangleNormals(){
    float length;
    for(int j=0;j<numberOfTriangles;j++){

        float sumX=0.0,sumY=0.0,sumZ=0.0;
        float x1,y1,z1,x2,y2,z2,length;

        x1 = Vertices[ 3* Triangles[3*j+1]   ] - Vertices[ 3* Triangles[3*j]   ];
        y1 = Vertices[ 3* Triangles[3*j+1]+1 ] - Vertices[ 3* Triangles[3*j]+1 ];
        z1 = Vertices[ 3* Triangles[3*j+1]+2 ] - Vertices[ 3* Triangles[3*j]+2 ];

        x2 = Vertices[ 3* Triangles[3*j+2]   ] - Vertices[ 3* Triangles[3*j]   ];
        y2 = Vertices[ 3* Triangles[3*j+2]+1 ] - Vertices[ 3* Triangles[3*j]+1 ];
        z2 = Vertices[ 3* Triangles[3*j+2]+2 ] - Vertices[ 3* Triangles[3*j]+2 ];

        sumX= y1*z2-z1*y2;
        sumY= z1*x2-x1*z2;
        sumZ= x1*y2-y1*x2;

        length  =  sqrt ( sumX*sumX + sumY*sumY + sumZ*sumZ );
        if(length==0){length=1;}

        TriangleNormals[3*j]=abs(sumX/length);
        TriangleNormals[3*j+1]=abs(sumY/length);
        TriangleNormals[3*j+2]=abs(sumZ/length);
    }
}

void Model::computeNormals(){

    for(int i=0;i<numberOfVertices;i++){
        float sumX=0.0,sumY=0.0,sumZ=0.0;
        float x1,y1,z1,x2,y2,z2,length;
        for(int j=0;j<numberOfTriangles;j++){
            if(i==Triangles[3*j]||i==Triangles[3*j+1]||i==Triangles[3*j+2]){

                sumX+= TriangleNormals[3*j];
                sumY+= TriangleNormals[3*j+1];
                sumZ+= TriangleNormals[3*j+2];
            }
        }

        length  =  sqrt ( sumX*sumX + sumY*sumY + sumZ*sumZ );
        if(length==0){length=1;}
        Normals[3*i]=abs(sumX/length);
        Normals[3*i+1]=abs(sumY/length);
        Normals[3*i+2]=abs(sumZ/length);
    }
}

void Model::setMatrix(glm::mat4 mat){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            rotationMatrix[i][j]=mat[i][j];
        }
    }
}

glm::mat4 Model::getMatrix(){ return rotationMatrix;}

float Model::get_x(){ return x;}
float Model::get_y(){ return y;}
float Model::get_z(){ return z;}
float Model::get_scale(){ return scale;}
void Model::set_x(float a){ x = a; }
void Model::set_y(float b){ y = b; }
void Model::set_z(float c){ z = c; }
void Model::set_scale(float s){ scale = s; }

unsigned int Model::get_vao(){ return VAO; }
unsigned int Model::get_lvao(){ return LVAO;}
unsigned int Model::get_cvao(){ return CVAO;}

// unsigned int Model::get_Texture(){ return TEXTURE;}
unsigned int* Model::get_vbo(){ return VBO; }

int Model::getNoVertices(){ return numberOfVertices; }
int Model::getNoTriangles(){ return numberOfTriangles; }
int Model::getNoSegments(){ return numberOfSegments; }



void Model::createLightmodel(int index){

    GLfloat vertices[]={  lightPosition.x , lightPosition.y , lightPosition.z };

    glGenVertexArrays(1, &LVAO);
    glBindVertexArray(LVAO);
    glGenBuffers(1, &LVBO);

    glBindBuffer(GL_ARRAY_BUFFER, LVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

glm::vec3 Model::getLightColor(){ return lightColor;}
void Model::setLightColor(glm::vec3 color){ lightColor=color;}

void Model::toggleLight(){
    if(lightColor.x==0.0f && lightColor.y==0.0f && lightColor.z==0.0f){ lightColor = templightColor;}
    else{ lightColor.x=0.0f; lightColor.y=0.0f; lightColor.z=0.0f;}
}

glm::vec3 Model::getLightPos(){
    return lightPosition;
}

void Model::printInfo(){

    cout<<"Vertices :-"<<endl;
    for(int i=0;i<numberOfVertices;i++){
        cout<<i<<" "<<Vertices[3*i]<<" "<<Vertices[3*i+1]<<" "<<Vertices[3*i+2]<<endl;
    }

    cout<<"Triangles :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        cout<<i<<" "<<Triangles[3*i]<<" "<<Triangles[3*i+1]<<" "<<Triangles[3*i+2]<<endl;
    }

    cout<<"TriangleNormals :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        cout<<i<<" "<<TriangleNormals[3*i]<<" "<<TriangleNormals[3*i+1]<<" "<<TriangleNormals[3*i+2]<<endl;
    }

    cout<<"Vetex Normals :-"<<endl;
    for(int i=0;i<numberOfVertices;i++){
        cout<<i<<" "<<Normals[3*i]<<" "<<Normals[3*i+1]<<" "<<Normals[3*i+2]<<endl;
    }

    cout<<"TriangleInradius :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        cout<<i<<" "<<InRadius[i]<<endl;
    }

    cout<<"TriangleInCenters :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        cout<<i<<" "<<InCenters[3*i]<<" "<<InCenters[3*i+1]<<" "<<InCenters[3*i+2]<<endl;
    }

    cout<<"SplatVertices :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        for(int j=0;j<=numberOfSegments;j++){
        cout<<i<<" "<<j<<" "<<SplatVertices[3*i*(numberOfSegments+1)+ 3*j]<<" "<<SplatVertices[3*i*(numberOfSegments+1)+ 3*j+1]<<" "<<SplatVertices[3*i*(numberOfSegments+1)+ 3*j+2]<<endl;
        }
    }

    cout<<"SplatTriangles :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        for(int j=0;j<numberOfSegments;j++){
        cout<<i<<" "<<j<<" "<<SplatTriangles[3*i*(numberOfSegments)+ 3*j]<<" "<<SplatTriangles[3*i*(numberOfSegments)+ 3*j+1]<<" "<<SplatTriangles[3*i*(numberOfSegments)+ 3*j+2]<<endl;
        }
    }
    cout<<"SplatColor :-"<<endl;
    for(int i=0;i<numberOfTriangles;i++){
        for(int j=0;j<=numberOfSegments;j++){
        cout<<i<<" "<<j<<" "<<SplatColor[3*i*(numberOfSegments+1)+ 3*j]<<" "<<SplatColor[3*i*(numberOfSegments+1)+ 3*j+1]<<" "<<SplatColor[3*i*(numberOfSegments)+ 3*j+2]<<endl;
        }
    }

}

void Model::computeInradius(){

    for(int index=0;index<numberOfTriangles;index++){

        int i1 = Triangles[3*index];
        int i2 = Triangles[3*index+1];
        int i3 = Triangles[3*index+2];

        glm::vec3 pos1,pos2,pos3 ;
        pos1.x=Vertices[3*i1];
        pos1.y=Vertices[3*i1+1];
        pos1.z=Vertices[3*i1+2];

        pos2.x=Vertices[3*i2];
        pos2.y=Vertices[3*i2+1];
        pos2.z=Vertices[3*i2+2];

        pos3.x=Vertices[3*i3];
        pos3.y=Vertices[3*i3+1];
        pos3.z=Vertices[3*i3+2];

        glm::vec3 side1 = pos2 - pos1;
        glm::vec3 side2 = pos3 - pos2;
        glm::vec3 side3 = pos3 - pos1;

        float len1 = sqrt(side1.x * side1.x + side1.y * side1.y + side1.z * side1.z);
        float len2 = sqrt(side2.x * side2.x + side2.y * side2.y + side2.z * side2.z);
        float len3 = sqrt(side3.x * side3.x + side3.y * side3.y + side3.z * side3.z);

        float s = (len1 + len2 + len3) / 2;
        float area = sqrt(s * (s - len1) * (s - len2) * (s - len3));
        InRadius[index] = (area) / (s);
    }
}

void Model::computeIncenters(){

    for(int index=0;index<numberOfTriangles;index++){

        int i1 = Triangles[3*index];
        int i2 = Triangles[3*index+1];
        int i3 = Triangles[3*index+2];

        glm::vec3 pos1,pos2,pos3 ;
        pos1.x=Vertices[3*i1];
        pos1.y=Vertices[3*i1+1];
        pos1.z=Vertices[3*i1+2];

        pos2.x=Vertices[3*i2];
        pos2.y=Vertices[3*i2+1];
        pos2.z=Vertices[3*i2+2];

        pos3.x=Vertices[3*i3];
        pos3.y=Vertices[3*i3+1];
        pos3.z=Vertices[3*i3+2];

        glm::vec3 side1 = pos2 - pos1;
        glm::vec3 side2 = pos3 - pos2;
        glm::vec3 side3 = pos3 - pos1;

        float len1 = sqrt(side1.x * side1.x + side1.y * side1.y + side1.z * side1.z);
        float len2 = sqrt(side2.x * side2.x + side2.y * side2.y + side2.z * side2.z);
        float len3 = sqrt(side3.x * side3.x + side3.y * side3.y + side3.z * side3.z);

        InCenters[3*index] = (len1 * pos3.x + len2 * pos1.x + len3 * pos2.x) / (len1 + len2 + len3);
        InCenters[3*index+1] = (len1 * pos3.y + len2 * pos1.y + len3 * pos2.y) / (len1 + len2 + len3);
        InCenters[3*index+2] = (len1 * pos3.z + len2 * pos1.z + len3 * pos2.z) / (len1 + len2 + len3);
        
    }

}

void Model::generateCircleVertices(){
    float theta;
    CircleVertices[0] = 0.0;
    CircleVertices[1] = 0.0;
    CircleVertices[2] = 0.0;
    for(int i = 1; i <=numberOfSegments; i++){
        theta = 2.0f * 3.1415926f * float(i) / float(numberOfSegments);
        CircleVertices[3*i] = glm::cos(theta);
        CircleVertices[3*i+1] = glm::sin(theta);
        CircleVertices[3*i+2] = 0.0;
    }
}

void Model::generateCircleTriangles(){ 

    for(int i = 0; i < numberOfSegments; i++){
        CircleTriangles[3*i] = 0;
        CircleTriangles[3*i+1] = i+1;
        CircleTriangles[3*i+2] = i+2;
    }
    CircleTriangles[3*numberOfSegments-1] = 1;
}

float Model::getAngle(int index){
    glm::vec3 normal;
    normal.x = TriangleNormals[3*index];
    normal.y = TriangleNormals[3*index+1];
    normal.z = TriangleNormals[3*index+2];
    float angle = acos(glm::dot(normal, glm::vec3(0.0f, 0.0f, -1.0f)));
    return angle;
}

glm::vec3 Model::getAxis(int index){
    glm::vec3 axis,normal;
    normal.x = TriangleNormals[3*index];
    normal.y = TriangleNormals[3*index+1];
    normal.z = TriangleNormals[3*index+2];
    axis = glm::cross(normal,glm::vec3(0, 0, 1));
    return axis;
}

void Model::generateSplatVertices(){
    float angle;
    glm::vec3 axis;
    for(int i=0;i<numberOfTriangles;i++){
        angle = getAngle(i);
        axis = getAxis(i);

        glm::mat4 translateCircle(1.0);
        glm::mat4 scaleCircle(1.0);
        glm::mat4 rotateCircle(1.0);

        glm::vec3 incenter;
        incenter.x = InCenters[3*i];
        incenter.y = InCenters[3*i+1];
        incenter.z = InCenters[3*i+2];

        translateCircle = glm::translate(translateCircle, incenter);
        scaleCircle = glm::scale(scaleCircle, glm::vec3(InRadius[i],InRadius[i],InRadius[i]));

        if(axis.x!=0||axis.y!=0||axis.z != 0) {
            rotateCircle = glm::rotate(rotateCircle, angle, glm::normalize(axis));
        }

        for(int j=0;j<=numberOfSegments;j++){

            glm::vec3 tempVertex = glm::vec3(CircleVertices[3*j],CircleVertices[3*j+1],CircleVertices[3*j+2]);
            tempVertex = glm::vec3(translateCircle * rotateCircle * scaleCircle * glm::vec4(tempVertex, 1.0f));

            SplatVertices[(numberOfSegments+1)*i*3 + 3*j] = tempVertex.x;
            SplatVertices[(numberOfSegments+1)*i*3 + 3*j+1] = tempVertex.y;
            SplatVertices[(numberOfSegments+1)*i*3 + 3*j+2] = tempVertex.z;

            // SplatVertices[(numberOfSegments+1)*i*3 + 3*j] = CircleVertices[3*j]*InRadius[i]+InCenters[3*i];
            // SplatVertices[(numberOfSegments+1)*i*3 + 3*j+1] = CircleVertices[3*j+1]*InRadius[i]+InCenters[3*i+1];
            // SplatVertices[(numberOfSegments+1)*i*3 + 3*j+2] = CircleVertices[3*j+2]*InRadius[i]+InCenters[3*i+2];


        }
    }
}

void Model::genetateSplatTriangles(){
    for(int i=0;i<numberOfTriangles;i++){
        for(int j=0;j<numberOfSegments;j++){
            // SplatTriangles[numberOfSegments*i*3 + 3*j] = (numberOfSegments+1)*i + CircleTriangles[3*j];
            // SplatTriangles[numberOfSegments*i*3 + 3*j+1] = (numberOfSegments+1)*i + CircleTriangles[3*j+1];
            // SplatTriangles[numberOfSegments*i*3 + 3*j+2] = (numberOfSegments+1)*i + CircleTriangles[3*j+2];

            SplatTriangles[numberOfSegments*i*3 + 3*j] = (numberOfSegments+1)*i + 0;
            SplatTriangles[numberOfSegments*i*3 + 3*j+1] = (numberOfSegments+1)*i + j+1;
            SplatTriangles[numberOfSegments*i*3 + 3*j+2] = (numberOfSegments+1)*i + j+2;
        }
        SplatTriangles[numberOfSegments*i*3 + 3*numberOfSegments-1] = (numberOfSegments+1)*i + 1;
    }
}

void Model::generateSplatColors(){
    for(int i=0;i<numberOfTriangles;i++){
        for(int j=0;j<=numberOfSegments;j++){
            SplatColor[(numberOfSegments+1)*i*3 + 3*j ] = TriangleNormals[3*i];
            SplatColor[(numberOfSegments+1)*i*3 + 3*j+1 ] = TriangleNormals[3*i+1];
            SplatColor[(numberOfSegments+1)*i*3 + 3*j+2 ] = TriangleNormals[3*i+2];
        }
    }
}

void Model::changeRendering(){
    renderId = (renderId+1)%2;
}

int Model::getRenderId(){ return renderId; }