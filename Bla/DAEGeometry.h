#ifndef DAEGEOMETRY_H
#define DAEGEOMETRY_H


#include <dae.h>
#include <dom/domNode.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCOLLADA.h>

#include "DAEGeometry.h"
#include "DAEMaterial.h"

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"


class DAEGeometry {
  
  domInstance_geometryRef geometry_;
  
public:
  
  typedef std::vector<glm::vec3> VertexList;
  
  DAEGeometry() { }
  
  DAEGeometry(domInstance_geometryRef geometry)
  : geometry_(geometry) { }
  
  DAEMaterial material() {    
    domInstance_materialRef instanceMaterial = geometry_->getBind_material()->getTechnique_common()->getInstance_material_array()[0];
    domMaterial* material = daeSafeCast<domMaterial>(instanceMaterial->getTarget().getElement());
    return DAEMaterial(material);
  }
  
  VertexList texels() {
    
    VertexList texels;
    domGeometry *geometry = daeSafeCast<domGeometry>(geometry_->getUrl().getElement());
    
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
        
        if (std::string("TEXCOORD").compare(offset->getSemantic()) == 0) {
          domSource* source = daeSafeCast<domSource>(offset->getSource().getElement());
          domFloat_arrayRef sourceDataArray = source->getFloat_array();
          domListOfFloats sourceData = sourceDataArray->getValue();
          
          domUint p = plist.get(pi);
          domFloat xIndex = p * 2;
          domFloat yIndex = (p * 2) + 1;
          
          float x = sourceData.get(xIndex);
          float y = sourceData.get(yIndex);
          
          glm::vec3 texel(x, y, 0);
          texels.push_back(texel);
        }
        
        offsetIndex++;          
        if (offsetIndex == offsets.getCount()) {
          offsetIndex = 0;
        }
      }
      
      
    }
    
    return texels;
  }
  
  VertexList normals() {
    VertexList normals;
    domGeometry *geometry = daeSafeCast<domGeometry>(geometry_->getUrl().getElement());
    
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
          
          glm::vec3 normal(x, z, y * -1);
          normals.push_back(normal);
        }
        
        offsetIndex++;          
        if (offsetIndex == offsets.getCount()) {
          offsetIndex = 0;
        }
      }
    }
    
    return normals;
  }
  
  VertexList vertices() {
    VertexList vertices;
    domGeometry *geometry = daeSafeCast<domGeometry>(geometry_->getUrl().getElement());
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
            
            glm::vec3 vertex(x, z, y * -1);
            vertices.push_back(vertex);
          }
        }
        
        offsetIndex++;          
        if (offsetIndex == offsets.getCount()) {
          offsetIndex = 0;
        }
      }
    }
    
    return vertices;
  }
  
};


#endif