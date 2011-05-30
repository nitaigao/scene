#ifndef SCENE_H
#define SCENE_H

#include <deque>
#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4

#include "Entity.h"
#include "SkyBox.h"
#include "Model.h"

#include "json/reader.h"
#include "json/elements.h"

using namespace json;

#include "String.h"

class Scene {
  
  typedef std::vector<Entity*> EntityList;
  EntityList entities;
  SkyBox skybox;
  
public:
  
  void init() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    FreeImage_Initialise();
    
    glClearColor(0.39,0.584,0.923,1.0);
  }
  
  void destroy() {
    FreeImage_DeInitialise();
  }
  
  void load(const std::string& path) {
  }
  
  void render(const glm::mat4& modelView, const glm::mat4& rotation) {    
    skybox.render(rotation);   
    
    for (EntityList::iterator i = entities.begin(); i != entities.end(); ++i) {
      (*i)->render(modelView);
    }
  };
  
};

#endif