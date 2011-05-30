#include "shader_pair.h"
#include <sstream>
#include "io.h"

std::string ShaderPair::vert() {
  std::stringstream vertName;
  vertName << pair_name_ << ".vert";
  return IO::readFile(vertName.str().c_str()); 
}

std::string ShaderPair::frag() {
  std::stringstream fragName;
  fragName << pair_name_ << ".frag";
  return IO::readFile(fragName.str().c_str()); 
}