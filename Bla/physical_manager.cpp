#include "physical_manager.h"
#include "entity_system.h"
#include "property.h"
#include "Physics.h"

static PhysicalManager* instance_ = 0;

PhysicalManager* PhysicalManager::manager() {
  if (!instance_) {
    instance_ = new PhysicalManager();
  }
  return instance_;
}

void PhysicalManager::destroy() {
  delete instance_;
  instance_ = 0;
}

void PhysicalManager::update(float delta_time) {
//  std::deque<Property*> physicals = EntitySystem::system()->query("physical");
//  
//  for (std::deque<Property*>::iterator i = physicals.begin(); i != physicals.end(); ++i) {
//    std::string shape = (*i)->attrs("shape");
//    
//    if (shape == "floor") {
//      
//    }    
//  }  
}

void PhysicalManager::reload() {
  std::deque<Property*> physicals = EntitySystem::system()->query("physical");
  
  for (std::deque<Property*>::iterator i = physicals.begin(); i != physicals.end(); ++i) {
    std::string data = (*i)->attrs("shape");
    Physics::system()->create_mesh(data, (*i)->label()); 
  }
}