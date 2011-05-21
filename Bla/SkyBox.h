#ifndef SKYBOX_H
#define SKYBOX_H

#include <GLUT/GLUT.h>
#include <string>

#include "Texture.h"

#include "Batch.h"
#include "DAEImporter.h"

class SkyBox {
  
  std::string top_;
  std::string bottom_;
  std::string left_;
  std::string right_;
  std::string front_;
  std::string back_;
  
  Batch* batch_;
  
  GLuint textureId_;
  
public:
  
  SkyBox() : batch_(0) { };
  
  void setTop(const std::string& top) { top_ = top; };
  void setBottom(const std::string& bottom) { bottom_ = bottom; };
  void setLeft(const std::string& left) { left_ = left; };
  void setRight(const std::string& right) { right_ = right; };
  void setFront(const std::string& front) { front_ = front; };
  void setBack(const std::string& back) { back_ = back; };
  
  void load() {
    
    static const int CUBE_SIZE = 6;
    
    const char* faces[CUBE_SIZE] = { top_.c_str(), bottom_.c_str(), left_.c_str(), right_.c_str(), front_.c_str(), back_.c_str() };
    
    GLenum cube[CUBE_SIZE] = { 
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };
    
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for(int i = 0; i < CUBE_SIZE; i++) { 
      unsigned int width, height;      
      BYTE* bits = Texture::loadImage(faces[i], &width, &height);
      glTexImage2D(cube[i], 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);      
    } 
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    batch_ = DAEImporter::load_dae("skybox.dae");
    batch_->initShaders("skybox");
    batch_->finalize();
  }
  
  void render(const glm::mat4& projection) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    batch_->render(glm::mat4(1.0f), projection);//, glm::mat4(1.0f));
  }
};

#endif