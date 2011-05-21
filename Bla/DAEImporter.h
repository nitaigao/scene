#ifndef DAEIMPORTER_H
#define DAEIMPORTER_H

#include <string>
#include "Batch.h"

class DAEImporter {
  
public:
  
  static Batch* load_dae(const std::string& path);
  
};

#endif