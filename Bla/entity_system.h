#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <string>
#include <deque>
#include <map>

class Property;

class EntitySystem
{
  
public:
  
  ~EntitySystem() { };
  
  EntitySystem()
  : last_property_id_(0) { }
  
  static EntitySystem* system();
  
  void destroy();
  
  void load(const std::string& filename);  
  
  void add_property(Property* property);
  
  Property* create_property();
  
  void next_id();
  
  std::deque<Property*> query(const std::string& type);
  
  Property* query(const std::string& type, unsigned int label);
  
private:
  
  EntitySystem(const EntitySystem & copy) { };
  EntitySystem & operator = (const EntitySystem & copy) { return *this; };
  
private:
  
  unsigned int last_property_id_;
  
  typedef std::deque<Property*> PropertyList;
  std::map<std::string, PropertyList> properties_;
  
};

#endif