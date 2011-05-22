#ifndef DAEIMPORTER_H
#define DAEIMPORTER_H

#include <string>
#include "Model.h"

#include <dae.h>

class DAEImporter {
  
public:
  
  static Model* load(const std::string& path);
  
};

class DAEFile {
  
public:
  
  static DAEFile* fromFile(const std::string& path);  
  void load(const std::string& path);
  
  
  Model* model();  
  
  DAE dae;
  domCOLLADA* root;
};


#endif