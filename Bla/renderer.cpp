#include "renderer.h"
#include <string>
#include <deque>
#include "property.h"
#include "entity_system.h"
#include "DAEImporter.h"
#include "Model.h"
#include <GLUT/GLUT.h>
#include "Camera.h"
#include "Physics.h"

static const std::string RENDERABLE_TYPE = "visible";
static const std::string CAMERA_TYPE = "camera";

static Renderer* instance_ = 0;

Renderer* Renderer::system() {
  if (!instance_) {
    instance_ = new Renderer();
  }
  return instance_;
}

void Renderer::destroy() {
  delete instance_;
  instance_ = 0;
}

void Renderer::reload() {
  typedef std::deque<Property*> PropertyList;
  
  PropertyList visibles = EntitySystem::system()->query(RENDERABLE_TYPE);  
  for (PropertyList::iterator i = visibles.begin(); i != visibles.end(); ++i) {
    std::string src = (*i)->attrs("model");
    DAEFile* file = DAEFile::fromFile(src);
    Model* model = file->model((*i)->label());
    std::string shader = (*i)->attrs("shader");
    model->initShaders(shader);
    
    models_.push_back(model);
  }  
  
  PropertyList cameras = EntitySystem::system()->query(CAMERA_TYPE);
  for (PropertyList::iterator i = cameras.begin(); i != cameras.end(); ++i) {
    Camera* camera = new Camera(width_, height_, (*i)->label());    
    cameras_.push_back(camera);
  }
}

void Renderer::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if (cameras_.size()) {  
    Camera* camera = cameras_.front();
    
    for (std::deque<Camera*>::iterator i = cameras_.begin(); i != cameras_.end(); ++i) {
      glm::vec3 origin = EntitySystem::system()->query("spatial", (*i)->label())->attrv3("origin");
      (*i)->set_position(origin);      
      
      float rx = EntitySystem::system()->query("spatial", (*i)->label())->attrf("rx");
      (*i)->rotateX(rx);
      
      float ry = EntitySystem::system()->query("spatial", (*i)->label())->attrf("ry");
      (*i)->rotateY(ry);
    }
    
    for (std::deque<Model*>::iterator i = models_.begin(); i != models_.end(); ++i) {
      glm::vec3 origin = EntitySystem::system()->query("spatial", (*i)->label())->attrv3("origin");      
      (*i)->set_position(origin);
      (*i)->render(camera->viewProjection());      
    }   
    
    Physics::system()->draw_debug(camera);

  } 
  
  glutSwapBuffers();
  glutPostRedisplay(); 
}

void Renderer::init(int width, int height) {
  width_ = width;
  height_ = height;
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  glClearColor(0.39,0.584,0.923,1.0);
}