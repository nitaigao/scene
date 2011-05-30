#include "property.h"
#include <assert.h>
#include <iostream>
#include <sstream>

void Property::attr(const std::string& key, const std::string& value) {
  string_values_[key] = value;
}

void Property::attr(const std::string& key, float value) {
  float_values_[key] = value;
}

void Property::attr(const std::string& key, const glm::vec4& value) {
  std::stringstream key_x;
  key_x << key << "_x";
  attr(key_x.str(), value.x);
  
  std::stringstream key_y;
  key_y << key << "_y";
  attr(key_y.str(), value.y);
  
  std::stringstream key_z;
  key_z << key << "_z";
  attr(key_z.str(), value.z);  
}

std::string Property::attrs(const std::string& key) {
  for (std::map<std::string, std::string>::iterator i = string_values_.begin(); i != string_values_.end(); ++i) {
    if ((*i).first == key) {
      return (*i).second;
    }
  }
  assert(0);
}

float Property::attrf(const std::string& key) {
  for (std::map<std::string, float>::iterator i = float_values_.begin(); i != float_values_.end(); ++i) {
    if ((*i).first == key) {
      return (*i).second;
    }
  }
  assert(0);  
}

glm::vec4 Property::attrv4(const std::string& key) {
  return glm::vec4(attrv3(key), 1.0f);
}

glm::vec3 Property::attrv3(const std::string& key) {
  std::stringstream key_x;
  key_x << key << "_x";
  float x = attrf(key_x.str());
  
  std::stringstream key_y;
  key_y << key << "_y";
  float y = attrf(key_y.str());
  
  std::stringstream key_z;
  key_z << key << "_z";
  float z = attrf(key_z.str());
  
  return glm::vec3(x, y, z);
}

unsigned int Property::label() {
  return id_;
}