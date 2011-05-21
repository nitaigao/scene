#ifndef ENTITY_H
#define ENTITY_H

#include "Batch.h"

class Entity {
  
  Batch* batch_;
  glm::vec3 position_;
  
public:
  
  Entity(Batch* batch, glm::vec3 position)
  : batch_(batch)
  , position_(position) {
    
  }
  
  void render(const glm::mat4& projection) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
    
    batch_->render(translation, projection);
  }
  
};

#endif