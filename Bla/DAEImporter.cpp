//
//  DAEImporter.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 01/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "DAEImporter.h"

#include <iostream>
#include <dae.h>
#include <dom/domNode.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCOLLADA.h>

Batch* DAEImporter::load_dae(const std::string& filename) {
  DAE dae;
  domCOLLADA* root = dae.open(filename.c_str());
  if (!root) {
    std::cout << "Document import failed.\n";
    return 0;
  }
  
  Batch* batch = new Batch();

  
  domCOLLADA::domSceneRef scene = root->getScene();
  domVisual_scene* vscene = daeSafeCast<domVisual_scene>(scene->getInstance_visual_scene()->getUrl().getElement());
  
  domNode_Array nodeArray = vscene->getNode_array();
  
  size_t nodeCount = nodeArray.getCount();
  
  for (int nodei = 0; nodei < nodeCount; nodei++) {
    domNodeRef node = nodeArray.get(nodei);
    
    domScale_Array scales = node->getScale_array();    
    domFloat3 scale = scales[0]->getValue();
    
    float scaleX = scale[0];
    float scaleY = scale[2];
    float scaleZ = scale[1];
    
    
    batch->setScale(scaleX, scaleY, scaleZ);
    
    domInstance_geometry_Array geometriesArray = node->getInstance_geometry_array();
    
    size_t geometryCount = geometriesArray.getCount();
    
    for (int geometryi = 0; geometryi < geometryCount; geometryi++) {
      domInstance_geometryRef instanceGeometry = geometriesArray.get(geometryi);
      domGeometry *geometry = daeSafeCast<domGeometry>(instanceGeometry->getUrl().getElement());
      
      domInstance_materialRef instanceMaterial = instanceGeometry->getBind_material()->getTechnique_common()->getInstance_material_array()[0];
      
      domMaterial* material = daeSafeCast<domMaterial>(instanceMaterial->getTarget().getElement());
      
      domEffect* effect =  daeSafeCast<domEffect>(material->getInstance_effect()->getUrl().getElement());
      
      domFx_profile_abstract_Array profileArray = effect->getFx_profile_abstract_array();
      
      domFx_profile_abstractRef fxProfile = profileArray.get(0);
      
      domProfile_COMMON* profileCommon = daeSafeCast<domProfile_COMMON>(fxProfile);
      
      domListOfFloats diffuse = profileCommon->getTechnique()->getPhong()->getDiffuse()->getColor()->getValue();
      
      batch->setDiffuse(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
      
      domListOfFloats specular = profileCommon->getTechnique()->getPhong()->getSpecular()->getColor()->getValue();

      batch->setSpecular(specular[0], specular[1], specular[2], specular[3]);
      
      domListOfFloats ambient = profileCommon->getTechnique()->getPhong()->getAmbient()->getColor()->getValue();
      
      batch->setAmbient(ambient[0], ambient[1], ambient[2], ambient[3]);
      
      
      domMeshRef mesh = geometry->getMesh();
      
      domPolylist_Array polylistArray = mesh->getPolylist_array();
      
      size_t polylistCount = polylistArray.getCount();
      for (int polylisti = 0; polylisti < polylistCount; polylisti++) {
        domPolylistRef polylist = polylistArray.get(polylisti);        
        
        domInputLocalOffset_Array offsets = polylist->getInput_array();
        
        domListOfUInts plist = polylist->getP()->getValue();
        
        
        int offsetIndex = 0;
        size_t pcount = plist.getCount();
        for (int pi = 0; pi < pcount; pi++) {
          
          domInputLocalOffsetRef offset = offsets.get(offsetIndex);
          
          if (std::string("VERTEX").compare(offset->getSemantic()) == 0) {
            domVertices* vertexSources = daeSafeCast<domVertices>(offset->getSource().getElement());
            domInputLocal_Array vertexSourceArray = vertexSources->getInput_array();
            
            size_t vertexSourceCount = vertexSourceArray.getCount();
            
            for (int vertexSourcei = 0; vertexSourcei < vertexSourceCount; vertexSourcei++) {
              domInputLocalRef inputLocal = vertexSourceArray.get(vertexSourcei);
              domSource* source = daeSafeCast<domSource>(inputLocal->getSource().getElement());
              
              domFloat_arrayRef sourceDataArray = source->getFloat_array();
              domListOfFloats sourceData = sourceDataArray->getValue();
              
              domUint p = plist.get(pi);
              domFloat xIndex = p * 3;
              domFloat yIndex = (p * 3) + 1;
              domFloat zIndex = (p * 3) + 2;
              
              float x = sourceData.get(xIndex);
              float y = sourceData.get(yIndex);
              float z = sourceData.get(zIndex);
              
              batch->addVertex(x, z , y * -1);
            }
          }
        
          if (std::string("NORMAL").compare(offset->getSemantic()) == 0) {
            domSource* normalSource = daeSafeCast<domSource>(offset->getSource().getElement());
            domFloat_arrayRef sourceDataArray = normalSource->getFloat_array();
            domListOfFloats sourceData = sourceDataArray->getValue();
            
            domUint p = plist.get(pi);
            domFloat xIndex = p * 3;
            domFloat yIndex = (p * 3) + 1;
            domFloat zIndex = (p * 3) + 2;
            
            float x = sourceData.get(xIndex);
            float y = sourceData.get(yIndex);
            float z = sourceData.get(zIndex);
            
            batch->addNormal(x, z, y * -1); 
          }
          
          offsetIndex++;          
          if (offsetIndex == offsets.getCount()) {
            offsetIndex = 0;
          }
        }
        
      }
    }
    
    batch->initShaders("shader");
    batch->finalize();
  }
  
  return batch;
}
