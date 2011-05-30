#include "shader.h"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"

enum {
  VERTEX = 0,
  NORMAL = 2,
  TEXTURE = 3,
  COLOR = 4
};

void Shader::compile_shader(const std::string& vertex, GLuint type) {
  GLuint shader = glCreateShader(type);  
  GLchar *src = (GLchar *)vertex.c_str();
  glShaderSource(shader, 1, (const GLchar**)&src, NULL);
  glCompileShader(shader);
  
  GLint testVal;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shader);
  }
  glAttachShader(id_, shader);
}

void Shader::bind_attributes() {
  glBindAttribLocation(id_, VERTEX, "vVertex");
  glBindAttribLocation(id_, NORMAL, "vNormal");
  glBindAttribLocation(id_, TEXTURE, "vTexCoords");
  glBindAttribLocation(id_, COLOR, "vColor");
}

void Shader::link() {
  glLinkProgram(id_);
  GLint testVal;
  glGetShaderiv(id_, GL_LINK_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(id_, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(id_);
  }
}

void Shader::compile(const std::string& vertex, const std::string& fragment) {
  id_ = glCreateProgram();
  compile_shader(vertex, GL_VERTEX_SHADER);
  compile_shader(fragment, GL_FRAGMENT_SHADER);  
  bind_attributes();
  link();
}

void Shader::use() {
  glUseProgram(id_);
}

void Shader::set_uniform(const std::string& name, int value) {
  GLint uniform = glGetUniformLocation(id_, name.c_str());
  glUniform1i(uniform, value);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
  GLint  uniform = glGetUniformLocation(id_, name.c_str());
  glUniform4fv(uniform, 1, glm::value_ptr(value));  
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  GLint  uniform = glGetUniformLocation(id_, name.c_str());
  glUniform3fv(uniform, 1, glm::value_ptr(value));    
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value) {
  GLint uniform = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));  
}

void Shader::set_uniform(const std::string& name, const glm::mat3& value) {
  GLint uniform = glGetUniformLocation(id_, name.c_str());
  glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(value));  
}