#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"

#include <deque>

#include <OpenGL/OpenGL.h>

#include "shader.h"

class Batch;

class Model {
  
  typedef std::deque<Batch*> BatchList;
  
  BatchList batches_;
  Shader shader_;
  
public:
  
  Model(unsigned int label)
  : label_(label) { };
  
  void initShaders(const std::string& name);
  
  void render(const glm::mat4& projectionMatrix);
  
  void addBatch(Batch* batch);
  
  void set_position(const glm::vec3& position);
  
  unsigned int label();
  
private:
  
  unsigned int label_;
  glm::vec3 position_;
};

#endif