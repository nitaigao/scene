#include "entity_system.h"
#include "scene_loader.h"
#include "property.h"
#include <assert.h>

static EntitySystem* instance_ = NULL;

EntitySystem* EntitySystem::system() {
  if (!instance_) {
    instance_ = new EntitySystem();
  }
  return instance_;
}

void EntitySystem::destroy() {
  delete instance_;
  instance_ = NULL;
}

void EntitySystem::load(const std::string& filename) {
  SceneLoader(filename).load(this);
}

void EntitySystem::add_property(Property* property) {  
  std::string type = property->attrs("type");
  properties_[type].push_back(property);  
}

Property* EntitySystem::create_property() {
  return new Property(last_property_id_);
}

void EntitySystem::next_id() {
  last_property_id_++;
}

std::deque<Property*> EntitySystem::query(const std::string& type) {
  return properties_[type];
}

Property* EntitySystem::query(const std::string& type, unsigned int label) {
  PropertyList properties = properties_[type];
  for (PropertyList::iterator i = properties.begin(); i != properties.end(); ++i) {
    if ((*i)->label() == label) {
      return (*i);
    }
  }
  assert(0);
}
