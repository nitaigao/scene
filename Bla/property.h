#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <map>
#include "glm/glm.hpp"

class Property
{
  
  std::map<std::string, float> float_values_;
  std::map<std::string, std::string> string_values_;
  
public:
  
  ~Property() { };
  
  Property(unsigned int id)
  : id_(id) { };
  
  void attr(const std::string& key, const std::string& value);
  void attr(const std::string& key, float value);
  void attr(const std::string& key, const glm::vec4& value);
  
  std::string attrs(const std::string& key);
  float attrf(const std::string& key);
  glm::vec4 attrv4(const std::string& key);
  glm::vec3 attrv3(const std::string& key);
  
  unsigned int label();
  
private:
  
  Property(const Property & copy) { };
  Property & operator = (const Property & copy) { return *this; };
  
private:
  
  unsigned int id_;
  
};

#endif