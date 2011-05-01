//
//  DAEImporter.h
//  Bla
//
//  Created by Nicholas Kostelnik on 01/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <string>
#include "Batch.h"

class DAEImporter {
  
public:
  
  static Batch* load_dae(const std::string& path);
  
};