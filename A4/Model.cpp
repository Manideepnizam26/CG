#include "Model.h"
using namespace  std;

Model::Model(){}

Model::Model(ifstream& plyfile,int index){

    scale = 0.2;
    x=-0.75+0.5*index;
    y=0.0;
    z=0.0;

    if(index==-1){scale=1;x=0; y=-0.35;}


    rotationMatrix=glm::mat4(1);
    modelMatrix = glm::mat4(1);
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
    SphericalTexels = new float[2*numberOfVertices];
    cylindricalTexels = new float[2*numberOfVertices];
    OpenGlTexels = new float[2*numberOfVertices];


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
//     initModel();
// }

    lightPosition.x = -0.75f+0.5f*index;
    lightPosition.y = 0.75f;
    lightPosition.z = 0.0f;


// void Model::initModel(){

    normalize();
    computeNormals();
    generateSpherical();
    generateCylindrical();
    generateOpengl();
    createLightmodel(index);


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(6, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[numberOfVertices]), Vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned int[numberOfTriangles]), Triangles, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float[numberOfVertices]), Normals,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[numberOfVertices]), SphericalTexels ,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[numberOfVertices]), cylindricalTexels,GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(float[numberOfVertices]), OpenGlTexels,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER,VBO[5]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    createTexture("wall.jpg",Texture[0]);
    createTexture("chessboard.jpeg",Texture[1]);
    createTexture("world.jpeg",Texture[2]);
    createTexture("selfie.jpeg",Texture[3]);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D,Texture[0]);
	glBindVertexArray(0);

    if(index==0){    lightColor.x = 0.0f;    }
    else if(index==1){  lightColor.y = 0.0f;  }
    else if(index==2){  lightColor.z = 0.0f; }
    else if(index==3){  lightColor.x=0.0f; lightColor.z=0;}
    templightColor = lightColor;

    // glBindVertexArray(VAO);
    // glGenTextures(1,&TEXTURE);
    // glBindTexture(GL_TEXTURE_2D,TEXTURE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("chessboard.jpg", &width, &height, &nrChannels, 0);
    // if (data){
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else{
    //     std::cout << "Failed to load texture" << std::endl;
    // }
    // stbi_image_free(data);
    // glBindTexture(GL_TEXTURE_2D,0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
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



void Model::computeNormals(){

    for(int i=0;i<numberOfVertices;i++){
        float sumX=0.0,sumY=0.0,sumZ=0.0;
        float x1,y1,z1,x2,y2,z2,length;
        for(int j=0;j<numberOfTriangles;j++){
            if(i==Triangles[3*j]||i==Triangles[3*j+1]||i==Triangles[3*j+2]){

                x1 = Vertices[ 3* Triangles[3*j+1]   ] - Vertices[ 3* Triangles[3*j]   ];
                y1 = Vertices[ 3* Triangles[3*j+1]+1 ] - Vertices[ 3* Triangles[3*j]+1 ];
                z1 = Vertices[ 3* Triangles[3*j+1]+2 ] - Vertices[ 3* Triangles[3*j]+2 ];

                x2 = Vertices[ 3* Triangles[3*j+2]   ] - Vertices[ 3* Triangles[3*j]   ];
                y2 = Vertices[ 3* Triangles[3*j+2]+1 ] - Vertices[ 3* Triangles[3*j]+1 ];
                z2 = Vertices[ 3* Triangles[3*j+2]+2 ] - Vertices[ 3* Triangles[3*j]+2 ];

                sumX+= y1*z2-z1*y2;
                sumY+= z1*x2-x1*z2;
                sumZ+= x1*y2-y1*x2;
            }
        }

        length  =  sqrt ( sumX*sumX + sumY*sumY + sumZ*sumZ );
        if(length==0){length=1;}
        Normals[3*i]=abs(sumX/length);
        Normals[3*i+1]=abs(sumY/length);
        Normals[3*i+2]=abs(sumZ/length);
    }
}

void Model::setmodelMatrix(glm::mat4 mat){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            modelMatrix[i][j]=mat[i][j];
        }
    }
}

void Model::setrotationMatrix(glm::mat4 mat){
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

// unsigned int Model::get_Texture(){ return TEXTURE;}
unsigned int* Model::get_vbo(){ return VBO; }

int Model::getNoVertices(){ return numberOfVertices; }
int Model::getNoTriangles(){ return numberOfTriangles; }



// void Model::createLightmodel(){

//     GLfloat vertices[]={  0.0f,0.0f, 0.0f };

//     glGenVertexArrays(1, &LVAO);
//     glBindVertexArray(LVAO);
//     glGenBuffers(1, &LVBO);

//     glBindBuffer(GL_ARRAY_BUFFER, LVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER,0);
//     glBindVertexArray(0);
// }


void Model::generateSpherical(){
    float length,projX,projY,projZ,radius = 2.0,s,t,PI=3.14159,TWOPI=6.283185308,a;

    for(int i=0;i<numberOfVertices;i++){
        length = sqrt(Vertices[3*i]*Vertices[3*i] + Vertices[3*i+1]*Vertices[3*i+1] + Vertices[3*i+2]*Vertices[3*i+2]);
        a = radius/length;
        projX = Vertices[3*i]*a;
        projY = Vertices[3*i+1]*a;
        projZ = Vertices[3*i+2]*a;
        t = acos(projZ/radius) / PI;
        if(projY>=0){ s = acos(projX/(radius * sin(PI*t))) / TWOPI; }
        else{ s = (PI + acos(projX/(radius * sin(PI*t)))) / TWOPI; }
        SphericalTexels[2*i]=s;
        SphericalTexels[2*i+1] = t;
    }
}

void Model::generateCylindrical(){
    float length,projX,projY,projZ,radius = 2.0,theta,PI=3.14159,s,t;
    for(int i=0;i<numberOfVertices;i++){
        length = sqrt(Vertices[3*i]*Vertices[3*i] + Vertices[3*i+1]*Vertices[3*i+1]);
        projX = Vertices[3*i]*radius/length;
        projY = Vertices[3*i+1]*radius/length;
        projZ = Vertices[3*i+2];
        theta = atan(projY/projX);
        s = (theta+PI/2)/PI;
        t = projZ/2 + 0.5;
        cylindricalTexels[2*i]=s;
        cylindricalTexels[2*i+1] = t;
    }
}

void Model::generateOpengl(){
    float s,t;
    for(int i=0;i<numberOfVertices;i++){

        s = Vertices[3*i]/2 + 0.5;
        t = Vertices[3*i+1]/2 + 0.5;


        OpenGlTexels[2*i]=s;
        OpenGlTexels[2*i+1]=t;
    }
}

void Model::createTexture(std::string filename, unsigned int &textureID){
    glBindVertexArray(VAO);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindVertexArray(0);
}

void Model::changeMapping(){

    mappingId = (mappingId+1)%3;
    glBindVertexArray(VAO);

    if(mappingId==0)     {    glBindBuffer(GL_ARRAY_BUFFER,VBO[5]);}
    else if(mappingId==1){ glBindBuffer(GL_ARRAY_BUFFER,VBO[4]);   }
    else if(mappingId==2){ glBindBuffer(GL_ARRAY_BUFFER,VBO[3]);}

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::changeTexture(){

    textureId = (textureId+1)%4;
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D,Texture[textureId]);
    glBindVertexArray(0);
}


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