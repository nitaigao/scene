#ifndef DAETEXTURE_H
#define DAETEXTURE_H

#include <dae.h>
#include <dom/domNode.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCOLLADA.h>

class DAETexture {
  
  domProfile_COMMON* profile_;
  
public:
  
  DAETexture(domProfile_COMMON* profile)
  : profile_(profile) {
    
  }
  
  std::string loadTextures(std::string name) {
    domCommon_newparam_type_Array newParams = profile_->getNewparam_array();
    size_t newParamsCount = newParams.getCount();
    
    for (int newParami = 0; newParami < newParamsCount; newParami++) {
      domCommon_newparam_typeRef param = newParams.get(newParami);
      
      if (std::string(param->getSid()).compare(name) == 0) {
        if (param->getSampler2D()) {
          std::string samplerName = param->getSampler2D()->getSource()->getValue();
          return loadTextures(samplerName);        
        }
        
        if (param->getSurface()) {
          domFx_surface_init_from_common_Array initFroms = param->getSurface()->getFx_surface_init_common()->getInit_from_array();
          size_t initFromCount = initFroms.getCount();
          
          for (int initFromi = 0; initFromi < initFromCount; initFromi++) {
            domFx_surface_init_from_commonRef initFromCommon = initFroms.get(initFromi);
            domImage* image = daeSafeCast<domImage>(initFromCommon->getValue().getElement());            
            return image->getInit_from()->getValue().getPath();
          }        
        }
      }    
    }   
    return "";
  }
  
  
  std::string filename() {
    domCommon_color_or_texture_type_complexType::domTextureRef texture = profile_->getTechnique()->getPhong()->getDiffuse()->getTexture();
    return loadTextures(texture->getTexture());
  }
  
};

#endif