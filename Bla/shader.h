#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <OpenGL/OpenGL.h>

#include "glm/glm.hpp"

class Shader {

public:
  
  Shader() : id_(0) { };
  
  void compile(const std::string& vertex, const std::string& fragment);
  
  void compile_shader(const std::string& vertex, GLuint type);
  
  void bind_attributes();  
  
  void link();
  
  void use();
  
  GLuint id() { return id_; };
  
  void set_uniform(const std::string& name, int value);
  
  void set_uniform(const std::string& name, const glm::vec4& value);
  
  void set_uniform(const std::string& name, const glm::vec3& value);
  
  void set_uniform(const std::string& name, const glm::mat4& value);
  
  void set_uniform(const std::string& name, const glm::mat3& value);
  
private:
  
  GLuint id_;
  
};

#endif
