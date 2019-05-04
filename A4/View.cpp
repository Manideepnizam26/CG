#include "View.h"


View::View(){ 
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraup);
}

void View::setCameraFront(glm::vec3 front){
    cameraFront = front;
}

void View::createShader(const char* vertexLocation, const char* fragmentLocation){
    string vertexString = readFile(vertexLocation);
    string fragmentString = readFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    compileShader(vertexCode, fragmentCode,ShaderID);
}


void View::createlightShader(const char* vertexLocation, const char* fragmentLocation){
    string vertexString = readFile(vertexLocation);
	string fragmentString = readFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	compileShader(vertexCode, fragmentCode,lightShaderID);
    // createLightmodel();
}

string View::readFile(const char* fileLocation){

    string content;
    ifstream fileStream(fileLocation, ios::in);
    string line = "";

    if (!fileStream.is_open()) {  printf("Failed to read %s! File doesn't exist.", fileLocation); return ""; }
    while (!fileStream.eof()) {  getline(fileStream, line);  content.append(line + "\n");  }
    fileStream.close();
    return content;
}


void View::compileShader(const char* vertexCode, const char* fragmentCode,GLuint &shaderid){

    shaderid = glCreateProgram();
    if(!shaderid){    cout<<"Error creating ShaderID."<<endl;    return;    }
    addShader(vertexCode, GL_VERTEX_SHADER,shaderid);
    addShader(fragmentCode, GL_FRAGMENT_SHADER,shaderid);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shaderid);
    glGetProgramiv(shaderid, GL_LINK_STATUS, &result);
    if (!result){
        glGetProgramInfoLog(shaderid, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }
    glValidateProgram(shaderid);
    glGetProgramiv(shaderid, GL_VALIDATE_STATUS, &result);
    if (!result){
        glGetProgramInfoLog(shaderid, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

void View::addShader(const char* ShaderIDCode, GLenum ShaderIDType,GLuint& shaderid){

    GLuint theShader = glCreateShader(ShaderIDType);
    const GLchar* theCode[1];
    GLint codeLength[1];

    theCode[0] = ShaderIDCode;
    codeLength[0] = strlen(ShaderIDCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result){
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d ShaderID: '%s'\n", ShaderIDType, eLog);
        return;
    }
    glAttachShader(shaderid, theShader);
    glDeleteShader(theShader);
}

GLuint View::getshader(){ return ShaderID; }
GLuint View::getLightShader(){ return lightShaderID; }


void View::updateScene(Scenegraph &graph){

}


void View::drawfloor(){
    
    glUseProgram(getshader());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLuint uniformModel = glGetUniformLocation(getshader(), "model");
    GLuint uniformModelColor = glGetUniformLocation(getshader(), "lightColor");
	glm::mat4 model = glm::mat4(1.0);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(uniformModelColor, 1.0,1.0,1.0,1.0);

	glBindVertexArray(FVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

}


void View::draw(Model &mymodel){

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    glUseProgram(getLightShader());
    GLuint uniformlightModel = glGetUniformLocation(getLightShader(), "lightmodel");
    GLuint uniformlightColor = glGetUniformLocation(getLightShader(), "lightColor");
    GLuint uniformlightview       = glGetUniformLocation(getLightShader(), "view");
    GLuint uniformlightprojection       = glGetUniformLocation(getLightShader(), "projection");


    glm::mat4 lightmodel = glm::mat4(1);
    glm::vec3 lightColor = mymodel.getLightColor();
    glUniformMatrix4fv(uniformlightModel, 1, GL_FALSE, glm::value_ptr(lightmodel));
    glUniform4f(uniformlightColor, lightColor.x,lightColor.y,lightColor.z,1.0);
    glUniformMatrix4fv(uniformlightview, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformlightprojection, 1,    GL_FALSE, glm::value_ptr(projection));


    // glBindVertexArray(LVAO);
    glBindVertexArray(mymodel.get_lvao());
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
    glUseProgram(0);

    glUseProgram(getshader());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLuint uniformModel = glGetUniformLocation(getshader(), "model");
    GLuint uniformModelColor = glGetUniformLocation(getshader(), "lightColor");
    GLuint uniformlightPosition = glGetUniformLocation(getshader(), "lightPosition");
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraup);

    GLuint uniformview       = glGetUniformLocation(getshader(), "view");
    GLuint uniformprojection       = glGetUniformLocation(getshader(), "projection");


	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3( mymodel.get_x() , mymodel.get_y() , mymodel.get_z() ));
	glm::mat4 rot = mymodel.getMatrix();
	model = model*rot;
	model = glm::scale(model, glm::vec3(mymodel.get_scale() , mymodel.get_scale(),mymodel.get_scale()));
    model = glm::rotate(model, mymodel.getAngle() , glm::vec3(0,1,0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(uniformModelColor, lightColor.x,lightColor.y,lightColor.z,1.0);
    glUniform3f(uniformlightPosition,mymodel.getLightPos().x,mymodel.getLightPos().y,mymodel.getLightPos().z);
    glUniformMatrix4fv(uniformview, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(uniformprojection, 1, GL_FALSE, glm::value_ptr(projection));

    // glBindTexture(GL_TEXTURE_2D, mymodel.get_Texture());
	glBindVertexArray(mymodel.get_vao());
	glDrawElements(GL_TRIANGLES, 3*mymodel.getNoTriangles(), GL_UNSIGNED_INT, 0);
    // glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
    
}