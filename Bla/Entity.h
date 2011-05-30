#ifndef ENTITY_H
#define ENTITY_H

#include "Batch.h"
#include "Model.h"

class Entity {
  
  Model* model_;
  glm::vec3 position_;
  
public:
  
  Entity(Model* model, glm::vec3 position)
  : model_(model)
  , position_(position) {
    
  }
  
  void render(const glm::mat4& projection) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
    
//    model_->render(translation, projection);
  }
  
};

#endif