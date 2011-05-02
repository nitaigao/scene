
#ifndef BATCH_H
#define BATCH_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

#include <OpenGL/OpenGL.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "IO.h"

class Batch {
  
  enum {
    VERTEX,
    NORMAL
  } BufferAttribs;
  
  std::vector<float> vertices;
  std::vector<float> normals;
  int indices;
  
  glm::vec3 scale_;
  glm::vec4 color_;
  
  GLuint vertexArrayObject;
  GLuint shaderProg;
  
public:
  
  inline void setScale(float x, float y, float z) {
    scale_.x = x;
    scale_.y = y;
    scale_.z = z;
  }
  
  inline void addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    indices++;
  }
  
  inline void addNormal(float x, float y, float z) {
    
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);    
  }
  
  inline void setDiffuse(float r, float g, float b, float a) {
    color_.r = r;
    color_.g = g;
    color_.b = b;
    color_.a = a;
  }
  
  void finalize() {
    glGenVertexArraysAPPLE(1, &vertexArrayObject);
    glBindVertexArrayAPPLE(vertexArrayObject);
    
    GLuint vertexBufferObject;
    GLuint normalBufferObject;

    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(VERTEX);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
  
  void initShaders() {
    shaderProg = glCreateProgram();
    GLint testVal;
    
    GLuint shaderVert = glCreateShader(GL_VERTEX_SHADER);  
    std::string vertSrc = IO::readFile("shader.vert");  
    std::clog << vertSrc << std::endl;
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
    glAttachShader(shaderProg, shaderVert);
    
    GLuint shaderFrag = glCreateShader(GL_FRAGMENT_SHADER);  
    std::string fragSrc = IO::readFile("shader.frag");
    GLchar *pFragSrc = (GLchar *)fragSrc.c_str();
    glShaderSource(shaderFrag, 1, (const GLchar**)&pFragSrc, NULL);
    glCompileShader(shaderFrag);
    
    glGetShaderiv(shaderFrag, GL_COMPILE_STATUS, &testVal);
    if (testVal == GL_FALSE) {
      char infoLog[1024];
      glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
      std::clog << infoLog << std::endl;
      glDeleteShader(shaderFrag);
    }
    glAttachShader(shaderProg, shaderFrag);
    
    glBindAttribLocation(shaderProg, VERTEX, "vVertex");
    glBindAttribLocation(shaderProg, NORMAL, "vNormal");
    
    glLinkProgram(shaderProg);
    glGetShaderiv(shaderFrag, GL_LINK_STATUS, &testVal);
    if (testVal == GL_FALSE) {
      char infoLog[1024];
      glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
      std::clog << infoLog << std::endl;
      glDeleteShader(shaderFrag);
    }
  }

  void render(const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix) {
    glUseProgram(shaderProg);
        
    glm::mat4 scale = glm::scale(modelViewMatrix, scale_);
    glm::mat4 rotation = glm::rotate(scale, 0.0f, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 mv(rotation);
    glm::mat4 mvp = projectionMatrix * mv;
    
    GLint uniformMVMatrix = glGetUniformLocation(shaderProg, "mvMatrix");
    glUniformMatrix4fv(uniformMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
    
    GLint  uniformLocation = glGetUniformLocation(shaderProg, "mvpMatrix");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = glm::inverse(glm::mat3(mv));
    GLint uniformNormalMatrix = glGetUniformLocation(shaderProg, "normalMatrix");
    glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    GLint  uniformColor = glGetUniformLocation(shaderProg, "diffuseColor");
    glUniform4fv(uniformColor, 1, glm::value_ptr(color_));
    
    glm::vec4 lightPosition(100, 100, 100, 1.0f);
    glm::vec4 eyeLightPosition = lightPosition * modelViewMatrix;
    glm::vec3 transformedLightPosition(eyeLightPosition);
    
    GLint uniformLightPosition = glGetUniformLocation(shaderProg, "vLightPosition");
    glUniform3fv(uniformLightPosition, 1, glm::value_ptr(transformedLightPosition));
    
    
    glBindVertexArrayAPPLE(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glDisableClientState(GL_VERTEX_ARRAY);    
  }
  
};

#endif