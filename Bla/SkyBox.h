#ifndef SKYBOX_H
#define SKYBOX_H

#include <GLUT/GLUT.h>
#include <string>

#include "Texture.h"

#include "DAEImporter.h"

class SkyBox {
  
  std::string top_;
  std::string bottom_;
  std::string left_;
  std::string right_;
  std::string front_;
  std::string back_;
  
  Model* model_;
  Texture texture_;
  
public:
  
  SkyBox() : model_(0) { texture_ = Texture(GL_TEXTURE_CUBE_MAP); };
  
  void setTop(const std::string& top) { top_ = top; };
  void setBottom(const std::string& bottom) { bottom_ = bottom; };
  void setLeft(const std::string& left) { left_ = left; };
  void setRight(const std::string& right) { right_ = right; };
  void setFront(const std::string& front) { front_ = front; };
  void setBack(const std::string& back) { back_ = back; };
  
  void load() {
    texture_.init(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture_.load(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top_.c_str());
    texture_.load(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom_.c_str());
    texture_.load(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left_.c_str());
    texture_.load(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right_.c_str());
    texture_.load(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front_.c_str());
    texture_.load(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back_.c_str());
        
    texture_.generateMipMap();
    
//    model_ = DAEFile::fromFile("skybox.dae")->model();
//    model_->initShaders("skybox");
  }
  
  void render(const glm::mat4& projection) {
    texture_.bind();    
//    model_->render(glm::mat4(1.0f), projection);//, glm::mat4(1.0f));
  }
};

#endif