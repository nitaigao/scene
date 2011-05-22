#ifndef SCENE_H
#define SCENE_H

#include <vector>
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
  
  void load(const std::string& path) {
    
    string jsonData = IO::readFile(path);
    jsonData = jsonData.replace("\xff", "");
    std::stringstream stream(jsonData);
    
    Object doc;
    
    try {
      Reader::Read(doc, stream);
    }
    catch (Exception e) {
      std::clog << e.what() << std::endl;
      throw e;
    }
    
    for (Object::const_iterator i = doc.Begin(); i != doc.End(); ++i) {
      
      string name = (*i).name;
      
      if (name == "skybox") {
        
        const Object& sb = (*i).element;
        
        skybox.setTop(String(sb["top"]));
        skybox.setBottom(String(sb["bottom"]));
        skybox.setLeft(String(sb["left"]));
        skybox.setRight(String(sb["right"]));
        skybox.setFront(String(sb["front"]));
        skybox.setBack(String(sb["back"]));
        
        skybox.load();
      }
      
      if (name == "entities") {
        
        const Array& jentities =  (*i).element;
        for(Array::const_iterator jentity = jentities.Begin(); jentity != jentities.End(); ++jentity) {
          
          std::string modelSrc = String((*jentity)["model"]);
          Number positionX = Number((*jentity)["position"]["x"]);
          Number positionY = Number((*jentity)["position"]["y"]);
          Number positionZ = Number((*jentity)["position"]["z"]);
          glm::vec3 position(positionX, positionY, positionZ);
          
          std::string shader = String((*jentity)["shader"]);
          
          Model* model = DAEImporter::load(modelSrc);
          
          Entity* entity = new Entity(model, position);
          entities.push_back(entity);
        }      
      }    
    }
  }
  
  void render(const glm::mat4& modelView, const glm::mat4& rotation) {    
//    skybox.render(rotation);   
    
    for (EntityList::iterator i = entities.begin(); i != entities.end(); ++i) {
      (*i)->render(modelView);
    }
  };
  
};

#endif