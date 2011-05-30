#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>

class EntitySystem;

class SceneLoader
{
  
  std::string path_;
  
public:
  
  ~SceneLoader() { };
  
  SceneLoader(const std::string& path)
  : path_(path) { }
    
  void load(EntitySystem* entity_system);  
  
private:
  
  SceneLoader(const SceneLoader & copy) { };
  SceneLoader & operator = (const SceneLoader & copy) { return *this; };
  
};

#endif