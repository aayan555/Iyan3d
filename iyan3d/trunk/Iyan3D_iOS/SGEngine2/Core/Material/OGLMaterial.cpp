//
//  Material.cpp
//  SGEngine2
//
//  Created by Vivek on 02/01/15.
//  Copyright (c) 2015 Smackall Games Pvt Ltd. All rights reserved.
//

#include "OGLMaterial.h"
#ifdef ANDROID
#include "../../../opengl.h"
#elif IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif


OGLMaterial::OGLMaterial(){
    
}
OGLMaterial::~OGLMaterial(){
    if(shaderProgram){
        uint32_t shaders[MAX_SHADERS_PER_MATERIAL];
        int32_t count;
        glGetAttachedShaders(shaderProgram, MAX_SHADERS_PER_MATERIAL, &count, shaders);
        count = min(count,MAX_SHADERS_PER_MATERIAL);
        for (GLint i = 0; i<count; ++i)
            glDeleteShader(shaders[i]);
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
    uniforms.clear();
    attributes.clear();
}
void OGLMaterial::AddAttributes(string name,DATA_TYPE type,uint32_t location,u16 bufIndex){
    attribute attr;
    attr.name = name;
    attr.type = type;
    attr.location = location;
    attributes.push_back(attr);
}
void OGLMaterial::AddProperty(string propertyName,NODE_PROPERTY property,DATA_TYPE type,u16 paramIndex,u16 count,uint32_t location,short nodeIndex){
    uniform uni;
    uni.location = location;
    uni.name = propertyName;
    uni.type = type;
    uni.property = property;
    uni.index = this->uniforms.size();
    uni.count = count;
    uni.nodeIndex = nodeIndex;
    uniforms.push_back(uni);
}
short OGLMaterial::setPropertyValue(string name,int *values,DATA_TYPE type,u16 count,u16 paramIndex,int nodeIndex,int renderTargetIndex){
    
    short uniformNodeIndex = NOT_EXISTS;
    for(int i = 0; i < uniforms.size();i++){
        if(uniforms[i].name == name){
            uniforms[i].values = values;
            uniformNodeIndex = i;
            break;
        }
    }
    if(uniformNodeIndex == NOT_EXISTS){
        uint location = glGetUniformLocation(shaderProgram,name.c_str());
        AddProperty(name,NODE_PROPERTY_USER_DEFINED,type,0,count,location,nodeIndex);
        uniformNodeIndex = uniforms.size()-1;
        uniforms[uniformNodeIndex].values = values;
    }
    if(uniformNodeIndex == NOT_EXISTS) {
        Logger::log(ERROR, "OGLMaterial", "Error in setting" + name + "Property");
    }
    return uniformNodeIndex;
}
short OGLMaterial::setPropertyValue(string name,float *values,DATA_TYPE type,u16 count,u16 paramIndex,int nodeIndex,int renderTargetIndex){
    short uniformNodeIndex = NOT_EXISTS;
    for(int i = 0; i < uniforms.size();i++){
        if(uniforms[i].name == name){
            uniforms[i].values = values;
            uniforms[i].count = count;
            uniformNodeIndex = i;
            break;
        }
    }
    if(uniformNodeIndex == NOT_EXISTS){
        uint location = glGetUniformLocation(shaderProgram,name.c_str());
        AddProperty(name,NODE_PROPERTY_USER_DEFINED,type,0,count,location,nodeIndex);
        uniformNodeIndex = uniforms.size()-1;
        uniforms[uniformNodeIndex].values = values;
    }
    if(uniformNodeIndex == NOT_EXISTS)
        Logger::log(ERROR, "OGLMaterial", "Error in setting" + name + "Property");
    return uniformNodeIndex;
}
void OGLMaterial::LoadShaders(string vShaderName,string fShaderName){
    GLuint vShaderHandle = CompileShader(vShaderName,GL_VERTEX_SHADER);
    GLuint fShaderHandle = CompileShader(fShaderName,GL_FRAGMENT_SHADER);
    shaderProgram = LinkShaders(vShaderHandle,fShaderHandle);
    glUseProgram(shaderProgram);
    GLint maxNameLength = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
    int maxAttrib = NOT_EXISTS;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &maxAttrib);
    for(int i = 0;i < maxAttrib;i++){
        GLint length = 0,size = 0;
        GLenum type; char attribName[maxNameLength];
        glGetActiveAttrib(shaderProgram,i,maxNameLength,&length,&size,&type,attribName);
        attribName[length] = '\0';
        string attribNameStr = string(attribName);
        AddAttributes(attribNameStr,Helper::getSGEngineDataType(type),CreateAttribute(shaderProgram,attribNameStr),0);
    }
    Logger::log(INFO,"OGLMaterial",vShaderName + "  " + fShaderName + "  Shaders Loaded Succesfully");
}
string OGLMaterial::getShaderAttributeNameByIndex(int i){
    int maxAttrib = NOT_EXISTS;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &maxAttrib);
    if(i >= maxAttrib)
        Logger::log(ERROR,"OGLMaterial:getAttributeNameByIndex","Index > than maxAttrib");
    
    GLint maxNameLength = 0;
    glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
    GLint length = 0,size = 0;
    GLenum type; char attribName[maxNameLength];
    glGetActiveAttrib(shaderProgram,i,maxNameLength,&length,&size,&type,attribName);
    attribName[length] = '\0';
    if(glGetError() == GL_INVALID_VALUE)
        Logger::log(ERROR,"OGLMaterial:getAttributeNameByIndex","GL_INVALID_VALUE");

    return string(attribName);
}
int OGLMaterial::getMaterialAttribIndexByName(string name){
    for(int i = 0;i < attributes.size();i++){
        if(attributes[i].name.compare(name) == 0)
            return i;
    }
    return NOT_EXISTS;
}
GLuint OGLMaterial::CompileShader(string shaderName,GLenum shaderType){
    // 1
    std::ifstream t(shaderName.c_str());
    std::stringstream buffer;
    buffer << t.rdbuf();
    string content = buffer.str();
    if (content.length() <= 0)
        Logger::log(ERROR,"OGLMaterial","Error Loading " + shaderName);
    GLuint shaderHandle = glCreateShader(shaderType);
    
    const char * shaderStringUTF8 = content.c_str();
    int shaderStringLength = (int)content.length();
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        Logger::log(ERROR,"OGLMaterial",messages);
    }
    Logger::log(INFO,"OGLMaterial",shaderName + " Compiled");
    return shaderHandle;
}
GLuint OGLMaterial::LinkShaders(GLuint vShaderHandle,GLuint fShaderHandle){
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vShaderHandle);
    glAttachShader(programHandle, fShaderHandle);
    glLinkProgram(programHandle);
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        Logger::log(ERROR,"OGLMaterial",messages);
    }
    return programHandle;
    
}
GLuint OGLMaterial::CreateAttribute(GLuint programHandle,string attribName){
    GLuint location = glGetAttribLocation(programHandle, attribName.c_str());
    glEnableVertexAttribArray(location);
    return location;
}
GLuint OGLMaterial::CreateUniform(GLuint programHandle,string uniName){
    return glGetUniformLocation(programHandle,uniName.c_str());
}