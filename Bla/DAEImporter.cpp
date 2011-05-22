//
//  DAEImporter.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 01/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "DAEImporter.h"

#include <dae.h>
#include <dom/domNode.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCOLLADA.h>

#include "Batch.h"

#include "DAENode.h"

void DAEFile::load(const std::string& path) {
  root = dae.open(path.c_str());
  if (!root) {
    std::cerr << "can't open " << path << std::endl;
  }
}

Model* DAEFile::model() {
  domCOLLADA::domSceneRef scene = root->getScene();
  domVisual_scene* vscene = daeSafeCast<domVisual_scene>(scene->getInstance_visual_scene()->getUrl().getElement());
  
  Model* model = new Model();
  domNode_Array nodeArray = vscene->getNode_array();
  size_t nodeCount = nodeArray.getCount();
  for (int nodei = 0; nodei < nodeCount; nodei++) {
    domNodeRef domNode = nodeArray.get(nodei);
    DAENode node = DAENode(domNode);
    
    Batch* batch = new Batch();
    batch->setScale(node.scale());
    
    if (!node.geometry().material().hasTexture()) {
      glm::vec4 diffuse = node.geometry().material().diffuse();
      batch->setDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
    } else {
      batch->setTexture(node.geometry().material().diffuseTexture().filename());
    }
    
    DAEGeometry::VertexList vertices = node.geometry().vertices();
    for (DAEGeometry::VertexList::iterator i = vertices.begin(); i != vertices.end(); ++i) {
      batch->addVertex((*i).x, (*i).y, (*i).z);
    }
    
    DAEGeometry::VertexList normals = node.geometry().normals();
    for (DAEGeometry::VertexList::iterator i = normals.begin(); i != normals.end(); ++i) {
      batch->addNormal((*i).x, (*i).y, (*i).z);
    }
    
    DAEGeometry::VertexList texels = node.geometry().texels();
    for (DAEGeometry::VertexList::iterator i = texels.begin(); i != texels.end(); ++i) {
      batch->addTexel((*i).s, (*i).t);
    }
    
    batch->finalize();
    model->addBatch(batch);
  }  
  return model;
};

DAEFile* DAEFile::fromFile(const std::string& path) {
  DAEFile* file = new DAEFile();
  file->load(path);
  return file;
}
