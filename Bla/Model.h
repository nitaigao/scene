#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"

#include <deque>

class Batch;

class Model {
  
  typedef std::deque<Batch*> BatchList;
  
  BatchList batches_;
  
public:
  
  void render(const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix);
  
  void addBatch(Batch* batch);
};

#endif