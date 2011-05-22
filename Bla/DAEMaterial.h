#ifndef DAEMATERIAL_H
#define DAEMATERIAL_H

#include <dae.h>
#include <dom/domNode.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCOLLADA.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"

#include "DAETexture.h"

class DAEMaterial {
  
  domMaterial* material_;
  
public:
  
  DAEMaterial(domMaterial* material) 
  : material_(material) { }
  
  bool hasTexture() {
    domEffect* effect =  daeSafeCast<domEffect>(material_->getInstance_effect()->getUrl().getElement());
    domFx_profile_abstract_Array profileArray = effect->getFx_profile_abstract_array();
    domFx_profile_abstractRef fxProfile = profileArray.get(0);
    domProfile_COMMON* profileCommon = daeSafeCast<domProfile_COMMON>(fxProfile);
    
    domCommon_color_or_texture_type_complexType::domTextureRef texture = profileCommon->getTechnique()->getPhong()->getDiffuse()->getTexture();
    return texture;    
  }
  
  glm::vec4 diffuse() {
    domEffect* effect =  daeSafeCast<domEffect>(material_->getInstance_effect()->getUrl().getElement());
    domFx_profile_abstract_Array profileArray = effect->getFx_profile_abstract_array();
    domFx_profile_abstractRef fxProfile = profileArray.get(0);
    domProfile_COMMON* profileCommon = daeSafeCast<domProfile_COMMON>(fxProfile);
    
    domCommon_color_or_texture_type_complexType::domColorRef diffuseColor = profileCommon->getTechnique()->getPhong()->getDiffuse()->getColor();
    domListOfFloats diffuse = diffuseColor->getValue();  
    return glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
  }
  
  DAETexture diffuseTexture() {
    domEffect* effect =  daeSafeCast<domEffect>(material_->getInstance_effect()->getUrl().getElement());
    domFx_profile_abstract_Array profileArray = effect->getFx_profile_abstract_array();
    domFx_profile_abstractRef fxProfile = profileArray.get(0);
    domProfile_COMMON* profileCommon = daeSafeCast<domProfile_COMMON>(fxProfile);
    return DAETexture(profileCommon);    
  }
  
};

#endif