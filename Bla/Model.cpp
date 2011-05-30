//
//  Model.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 22/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Model.h"

#include "Batch.h"
#include "shader_pair.h"

void Model::initShaders(const std::string& name) {
  ShaderPair pair(name);
  shader_.compile(pair.vert(), pair.frag());  
}


void Model::render(const glm::mat4& projectionMatrix) {
  glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), position_);
  
  for (BatchList::iterator i = batches_.begin(); i != batches_.end(); ++i) {
    (*i)->render(shader_, modelMatrix, projectionMatrix);
  }
}

void Model::addBatch(Batch* batch) {
  batches_.push_back(batch);
}

unsigned int Model::label() {
  return label_;
}

void Model::set_position(const glm::vec3& position) {
  position_ = position;
}