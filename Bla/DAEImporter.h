#ifndef DAEIMPORTER_H
#define DAEIMPORTER_H

#include <string>
#include "Model.h"

#include <dae.h>

class DAEFile {
  
  DAE dae;
  domCOLLADA* root;
  
public:
  
  static DAEFile* fromFile(const std::string& path);  
  void load(const std::string& path);
    
  Model* model();  
  
};


#endif