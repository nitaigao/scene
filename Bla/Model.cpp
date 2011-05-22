//
//  Model.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 22/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Model.h"

#include "Batch.h"

void Model::initShaders(const std::string& name) {
  
  shader_ = glCreateProgram();
  GLint testVal;
  
  GLuint shaderVert = glCreateShader(GL_VERTEX_SHADER);  
  std::stringstream vertName;
  vertName << name << ".vert";
  std::string vertSrc = IO::readFile(vertName.str().c_str());  
  
  GLchar *pVertSrc = (GLchar *)vertSrc.c_str();
  glShaderSource(shaderVert, 1, (const GLchar**)&pVertSrc, NULL);
  glCompileShader(shaderVert);
  
  
  glGetShaderiv(shaderVert, GL_COMPILE_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderVert, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shaderVert);
  }
  glAttachShader(shader_, shaderVert);
  
  GLuint shaderFrag = glCreateShader(GL_FRAGMENT_SHADER);  
  std::stringstream fragName;
  fragName << name << ".frag";
  std::string fragSrc = IO::readFile(fragName.str().c_str());
  GLchar *pFragSrc = (GLchar *)fragSrc.c_str();
  glShaderSource(shaderFrag, 1, (const GLchar**)&pFragSrc, NULL);
  glCompileShader(shaderFrag);
  
  glGetShaderiv(shaderFrag, GL_COMPILE_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
    std::clog << infoLog << std:: endl;
    glDeleteShader(shaderFrag);
  }
  glAttachShader(shader_, shaderFrag);
  
  glBindAttribLocation(shader_, VERTEX, "vVertex");
  glBindAttribLocation(shader_, NORMAL, "vNormal");
  glBindAttribLocation(shader_, TEXTURE, "vTexCoords");
  
  glLinkProgram(shader_);
  glGetShaderiv(shaderFrag, GL_LINK_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shaderFrag);
  }
}


void Model::render(const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix) {
  glUseProgram(shader_);
  
  for (BatchList::iterator i = batches_.begin(); i != batches_.end(); ++i) {
    (*i)->render(shader_, modelViewMatrix, projectionMatrix);
  }
}

void Model::addBatch(Batch* batch) {
  batches_.push_back(batch);
}