
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
#include "Texture.h"

class Batch {
  
  enum {
    VERTEX = 0,
    NORMAL = 2,
    TEXTURE = 3
  } BufferAttribs;
  
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texels_;
  int indices;
  
  glm::vec3 scale_;
  glm::vec4 diffuse_;
  glm::vec4 specular_;
  glm::vec4 ambient_;
  
  GLuint vertexArrayObject;
  GLuint shaderProg;
  
  GLuint textureId_;
  
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
  
  inline void addTexel(float s, float t) {
    texels_.push_back(s);
    texels_.push_back(t);
  }
  
  inline void setDiffuse(float r, float g, float b, float a) {
    diffuse_.r = r;
    diffuse_.g = g;
    diffuse_.b = b;
    diffuse_.a = a;
  }
  
  inline void setAmbient(float r, float g, float b, float a) {
    ambient_.r = r;
    ambient_.g = g;
    ambient_.b = b;
    ambient_.a = a;
  }
  
  inline void setSpecular(float r, float g, float b, float a) { 
    specular_.r = r;
    specular_.g = g;
    specular_.b = b;
    specular_.a = a;    
  }
  
  inline void setTexture(const std::string& path) {
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    unsigned int width, height;
    
    BYTE* bits = Texture::loadImage(path, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
    glGenerateMipmap(GL_TEXTURE_2D);    
  }
  
  void finalize() {
    glGenVertexArraysAPPLE(1, &vertexArrayObject);
    glBindVertexArrayAPPLE(vertexArrayObject);
    
    GLuint vertexBufferObject;
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(VERTEX);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    GLuint normalBufferObject;
        
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    GLuint textureBufferObject;
    
    glGenBuffers(1, &textureBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
    glBufferData(GL_ARRAY_BUFFER, texels_.size() * sizeof(float), &texels_[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
    glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
  
  void initShaders(const std::string& name) {
      
    shaderProg = glCreateProgram();
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
    glAttachShader(shaderProg, shaderVert);
    
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
    glAttachShader(shaderProg, shaderFrag);
    
    glBindAttribLocation(shaderProg, VERTEX, "vVertex");
    glBindAttribLocation(shaderProg, NORMAL, "vNormal");
    glBindAttribLocation(shaderProg, TEXTURE, "vTexCoords");
    
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
    
    glBindTexture(GL_TEXTURE_2D, textureId_);
        
    glm::mat4 scale = glm::scale(modelViewMatrix, scale_);
    glm::mat4 mv(scale);
    glm::mat4 mvp = projectionMatrix * mv;
    
//    glm::mat4 inverseCameraRotation = glm::inverse(rotation);
//    GLint uniformInverseCamera = glGetUniformLocation(shaderProg, "mInverseCamera");
//    glUniformMatrix4fv(uniformInverseCamera, 1, GL_FALSE, glm::value_ptr(inverseCameraRotation));
    
    GLint uniformColorMap = glGetUniformLocation(shaderProg, "colorMap");
    glUniform1i(uniformColorMap, 0);
    
    GLint uniformMVMatrix = glGetUniformLocation(shaderProg, "mvMatrix");
    glUniformMatrix4fv(uniformMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
    
    GLint  uniformLocation = glGetUniformLocation(shaderProg, "mvpMatrix");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mv));
    GLint uniformNormalMatrix = glGetUniformLocation(shaderProg, "normalMatrix");
    glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        
    GLint  uniformColor = glGetUniformLocation(shaderProg, "diffuseColor");
    glUniform4fv(uniformColor, 1, glm::value_ptr(diffuse_));
    
    GLint uniformSpecColor = glGetUniformLocation(shaderProg, "specularColor");
    glUniform4fv(uniformSpecColor, 1, glm::value_ptr(specular_));
    
    GLint uniformAmbientColor = glGetUniformLocation(shaderProg, "ambientColor");
    glUniform4fv(uniformAmbientColor, 1, glm::value_ptr(ambient_));

    
    glm::vec4 lightPosition(100, 100, 100, 1.0f);
    GLint uniformLightPosition = glGetUniformLocation(shaderProg, "vLightPosition");
    glUniform3fv(uniformLightPosition, 1, glm::value_ptr(lightPosition));
    
    
    
    glBindVertexArrayAPPLE(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glDisableClientState(GL_VERTEX_ARRAY);    
  }
};

#endif