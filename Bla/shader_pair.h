#ifndef SHADERPAIR_H
#define SHADERPAIR_H

#include <string>

class ShaderPair {
  
public:
  
  ShaderPair(const std::string& pair_name)
  : pair_name_(pair_name) { };
  
  std::string vert();
  std::string frag();
  
private:
  
  std::string pair_name_;
  
};

#endif
