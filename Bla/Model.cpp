//
//  Model.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 22/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Model.h"

#include "Batch.h"

void Model::render(const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix) {
  for (BatchList::iterator i = batches_.begin(); i != batches_.end(); ++i) {
    (*i)->render(modelViewMatrix, projectionMatrix);
  }
}

void Model::addBatch(Batch* batch) {
  batches_.push_back(batch);
}