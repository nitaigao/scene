#ifndef DAENODE_H
#define DAENODE_H

#include <dom/domNode.h>
#include "DAEGeometry.h"

class DAENode {
  
  domNodeRef node_;
  
public:
  
  DAENode(domNodeRef node)
  : node_(node) { }
  
  glm::vec3 scale() {
    domScale_Array scales = node_->getScale_array();    
    domFloat3 scale = scales[0]->getValue(); 
    return glm::vec3(scale[0], scale[2], scale[1]);
  }
  
  DAEGeometry geometry() {
    domInstance_geometry_Array geometriesArray = node_->getInstance_geometry_array();
    size_t geometryCount = geometriesArray.getCount();
    for (int geometryi = 0; geometryi < geometryCount; geometryi++) {
      domInstance_geometryRef instanceGeometry = geometriesArray.get(geometryi);
      return DAEGeometry(instanceGeometry);
    }
    return DAEGeometry();
  }
};

#endif
