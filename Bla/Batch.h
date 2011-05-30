
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
#include "shader.h"

enum {
  VERTEX = 0,
  NORMAL = 2,
  TEXTURE = 3,
  COLOR = 4
};

class Batch {
  
  
  std::vector<float> vertices_;
  std::vector<float> normals_;
  std::vector<float> texels_;
  std::vector<float> colors_;
  int indices_;
  
  glm::vec3 scale_;
  glm::vec4 diffuse_;
  glm::vec4 specular_;
  glm::vec4 ambient_;
  
  GLuint vertexArrayObject;
  
  GLuint vertex_buffer_object_;
  GLuint normal_buffer_object_;
  GLuint texture_buffer_object_;
  GLuint color_buffer_object_;
  
  GLuint textureId_;
  
public:
  
  Batch() : indices_(0) { };
  
  
  inline void setScale(const glm::vec3& scale) {
    scale_.x = scale.x;
    scale_.y = scale.y;
    scale_.z = scale.z;
  }
    
  inline void addVertex(float x, float y, float z) {
    vertices_.push_back(x);
    vertices_.push_back(y);
    vertices_.push_back(z);
    indices_++;
  }
  
  inline void addNormal(float x, float y, float z) {
    normals_.push_back(x);
    normals_.push_back(y);
    normals_.push_back(z);    
  }
  
  inline void addTexel(float s, float t) {
    texels_.push_back(s);
    texels_.push_back(t);
  }
  
  inline void addColor(float r, float g, float b) {
    colors_.push_back(r);
    colors_.push_back(g);
    colors_.push_back(b);
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
  
  void set_data() {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), &vertices_[0], GL_DYNAMIC_DRAW);
  
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(float), &normals_[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, texels_.size() * sizeof(float), &texels_[0], GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), &colors_[0], GL_DYNAMIC_DRAW);
  }
  
  void flush_buffers() {
    vertices_.clear();
    normals_.clear();
    texels_.clear();
    colors_.clear();
    indices_ = 0;
  }
  
  void finalize() {
    glGenVertexArraysAPPLE(1, &vertexArrayObject);
    glBindVertexArrayAPPLE(vertexArrayObject);
    
    glGenBuffers(1, &vertex_buffer_object_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), &vertices_[0], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(VERTEX);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
    glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
    glGenBuffers(1, &normal_buffer_object_);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(float), &normals_[0], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_object_);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
      
    glGenBuffers(1, &texture_buffer_object_);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, texels_.size() * sizeof(float), &texels_[0], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer_object_);
    glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &color_buffer_object_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object_);
    glBufferData(GL_ARRAY_BUFFER, colors_.size() * sizeof(float), &colors_[0], GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object_);
    glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
  }
  
  void render(GLenum mode, Shader& shader, const glm::mat4& modelMatrix, const glm::mat4& projectionMatrix) {
    
    shader.use();
    glBindTexture(GL_TEXTURE_2D, textureId_);
    
    glm::mat4 scale = glm::scale(modelMatrix, scale_);
    glm::mat4 mv(scale);
    glm::mat4 mvp = projectionMatrix * mv;
    
    shader.set_uniform("colorMap", 0);
    shader.set_uniform("mvMatrix", mv);
    shader.set_uniform("mvpMatrix", mvp);
    shader.set_uniform("diffuseColor", diffuse_);
    shader.set_uniform("specularColor", specular_);
    shader.set_uniform("ambientColor", ambient_);
    
    glm::vec3 lightPosition(100, 100, 100);
    shader.set_uniform("vLightPosition", lightPosition);
    
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mv));
    shader.set_uniform("normalMatrix", normalMatrix);
        
    glEnableClientState(GL_VERTEX_ARRAY);        
    glBindVertexArrayAPPLE(vertexArrayObject);
    glDrawArrays(mode, 0, indices_);
    glDisableClientState(GL_VERTEX_ARRAY);        
  }
  
  void render(Shader& shader, const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix) {
    render(GL_TRIANGLES, shader, modelViewMatrix, projectionMatrix);
  }
};

#endif