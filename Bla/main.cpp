//
//  main.cpp
//  Bla
//
//  Created by Nicholas Kostelnik on 29/04/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <sys/time.h>
#include <FreeImage.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


//#include "math3d.h"

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#define WINDOW_X 800
#define WINDOW_Y 600

struct timeval _tstart, _tend;

GLuint frontTexture = 0;

GLuint shaderVert;
GLuint shaderFrag;
GLuint shaderProg;

enum {
  VERTEX,
  NORMAL
};


#include <iostream>
#include <Collada14Dom/dae.h>
#include <Collada14Dom/dom/domNode.h>
#include <Collada14Dom/dom/domCOLLADA.h>

#include <vector>

float movZ;
float movY;
float movX;

float rotX;
float rotY;

class Batch {
  
  std::vector<float> vertices;
  std::vector<float> normals;
  int indices;
  
  glm::vec3 scale_;
  
  GLuint vertexArrayObject;
  GLuint vertexBufferObject;
  GLuint normalBufferObject;

  
public:
  
  void setScale(float x, float y, float z) {
    scale_.x = x;
    scale_.y = y;
    scale_.z = z;
  }
  
  void addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    indices++;
  }
  
  void addNormal(float x, float y, float z) {
    
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);    
  }
  
  void finalize() {
    glGenVertexArraysAPPLE(1, &vertexArrayObject);
    glBindVertexArrayAPPLE(vertexArrayObject);
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    std::clog << vertexBufferObject << std::endl;
    
    glEnableVertexAttribArray(VERTEX);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
  
  void render() {
    glUseProgram(shaderProg);
    
    glm::mat4 projection = glm::perspective(75.0f, float(WINDOW_X) / float(WINDOW_Y), 0.5f, 100.f);
    
    glm::mat4 eyeRotationY = glm::rotate(glm::mat4(1.0f), rotY, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 eyeRotationX = glm::rotate(eyeRotationY, rotX, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 eyeTranslation = glm::translate(eyeRotationX, glm::vec3(0.0f + movX, -2.0f + movY, -2.0f + movZ));
    

    glm::mat4 scale = glm::scale(eyeTranslation, scale_);
    glm::mat4 rotation = glm::rotate(scale, 0.0f, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 mv(rotation);
    glm::mat4 mvp = projection * mv;
    
    GLint uniformMVMatrix = glGetUniformLocation(shaderProg, "mvMatrix");
    glUniformMatrix4fv(uniformMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
    
    GLint  uniformLocation = glGetUniformLocation(shaderProg, "mvpMatrix");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = glm::inverse(glm::mat3(mv));
    GLint uniformNormalMatrix = glGetUniformLocation(shaderProg, "normalMatrix");
    glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
    float vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLint  uniformColor = glGetUniformLocation(shaderProg, "diffuseColor");
    glUniform4fv(uniformColor, 1, vRed);
    
    glm::vec4 lightPosition(100, 100, 100, 1.0f);
    glm::vec4 eyeLightPosition = lightPosition * eyeTranslation;
    glm::vec3 transformedLightPosition(eyeLightPosition);
    
    GLint uniformLightPosition = glGetUniformLocation(shaderProg, "vLightPosition");
    glUniform3fv(uniformLightPosition, 1, glm::value_ptr(transformedLightPosition));

    
    glBindVertexArrayAPPLE(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glDisableClientState(GL_VERTEX_ARRAY);    
  }
  
};

typedef std::vector<Batch*> BatchList;
BatchList batches;

void load_dae(const std::string& filename) {
  DAE dae;
  domCOLLADA* root = dae.open(filename.c_str());
  if (!root) {
    std::cout << "Document import failed.\n";
    return;
  }
  
  domCOLLADA::domSceneRef scene = root->getScene();
  domVisual_scene* vscene = daeSafeCast<domVisual_scene>(scene->getInstance_visual_scene()->getUrl().getElement());
  
  domNode_Array nodeArray = vscene->getNode_array();
  
  int nodeCount = nodeArray.getCount();
  
  for (int nodei = 0; nodei < nodeCount; nodei++) {
    domNodeRef node = nodeArray.get(nodei);
    
    domScale_Array scales = node->getScale_array();    
    domFloat3 scale = scales[0]->getValue();
    
    float scaleX = scale[0];
    float scaleY = scale[2];
    float scaleZ = scale[1];
    
    Batch* batch = new Batch();
    
    batch->setScale(scaleX, scaleY, scaleZ);
    
    domInstance_geometry_Array geometriesArray = node->getInstance_geometry_array();
    
    int geometryCount = geometriesArray.getCount();
    
    for (int geometryi = 0; geometryi < geometryCount; geometryi++) {
      domInstance_geometryRef instanceGeometry = geometriesArray.get(geometryi);
      domGeometry *geometry = daeSafeCast<domGeometry>(instanceGeometry->getUrl().getElement());
      domMeshRef mesh = geometry->getMesh();
      
      domPolylist_Array polylistArray = mesh->getPolylist_array();
      
      int polylistCount = polylistArray.getCount();
      for (int polylisti = 0; polylisti < polylistCount; polylisti++) {
        domPolylistRef polylist = polylistArray.get(polylisti);        
        
        domInputLocalOffset_Array offsets = polylist->getInput_array();
        
        domListOfUInts plist = polylist->getP()->getValue();
        
        int offsetIndex = 0;
        int pcount = plist.getCount();
        for (int pi = 0; pi < pcount; pi++) {
          
          domInputLocalOffsetRef offset = offsets.get(offsetIndex);
          
          if (std::string("VERTEX").compare(offset->getSemantic()) == 0) {
            domVertices* vertexSources = daeSafeCast<domVertices>(offset->getSource().getElement());
            domInputLocal_Array vertexSourceArray = vertexSources->getInput_array();
            
            int vertexSourceCount = vertexSourceArray.getCount();
            
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
    
    batch->finalize();
    batches.push_back(batch);
    
  }
}

std::string readFile(const std::string& filename) {
  std::ifstream ifs (filename.c_str(), std::ifstream::in );
  std::stringstream output;
  
  while (ifs.good()) {
    output << (char) ifs.get();
  }
  
  return output.str();
}

GLuint initShaders() {
  GLuint shaderProgram = glCreateProgram();
  GLint testVal;
  
  shaderVert = glCreateShader(GL_VERTEX_SHADER);  
  std::string vertSrc = readFile("shader.vert");  
  GLchar *pVertSrc = (GLchar *)vertSrc.c_str();
  glShaderSource(shaderVert, 1, (const GLchar**)&pVertSrc, NULL);
  glCompileShader(shaderVert);
  
  glGetShaderiv(shaderVert, GL_COMPILE_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderVert, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shaderVert);
  }
  glAttachShader(shaderProgram, shaderVert);
  
  shaderFrag = glCreateShader(GL_FRAGMENT_SHADER);  
  std::string fragSrc = readFile("shader.frag");
  GLchar *pFragSrc = (GLchar *)fragSrc.c_str();
  glShaderSource(shaderFrag, 1, (const GLchar**)&pFragSrc, NULL);
  glCompileShader(shaderFrag);
  
  glGetShaderiv(shaderFrag, GL_COMPILE_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shaderFrag);
  }
  glAttachShader(shaderProgram, shaderFrag);
  
  glBindAttribLocation(shaderProgram, VERTEX, "vVertex");
  glBindAttribLocation(shaderProgram, NORMAL, "vNormal");
  
  glLinkProgram(shaderProgram);
  glGetShaderiv(shaderFrag, GL_LINK_STATUS, &testVal);
  if (testVal == GL_FALSE) {
    char infoLog[1024];
    glGetShaderInfoLog(shaderFrag, 1024, NULL, infoLog);
    std::clog << infoLog << std::endl;
    glDeleteShader(shaderFrag);
  }
  
  return shaderProgram;
}

void render() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  for (BatchList::iterator i = batches.begin(); i != batches.end(); ++i) {
    (*i)->render();
  }
  
  glutSwapBuffers();
  glutPostRedisplay();
}

void update() {
  //  gettimeofday(&_tend,0);
  //  double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
  //  double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
  //  float deltaTime = float(t2-t1);
  //  _tstart = _tend;
}

int lastX = 0;
int lastY = 0;
bool firstInputPass = true;

void resetMouse() {
  glutWarpPointer(WINDOW_X / 2, WINDOW_Y / 2);
}

void passiveMotion(int x, int y) {
  
  if (!firstInputPass) {
    
    int xDelta = lastX - x;
    int yDelta = lastY - y;
    
    
    rotX -= xDelta * 0.1f;
    rotY -= yDelta * 0.1f;    
  }
  
  
  lastX = x;
  lastY = y;
  
  firstInputPass = false;
  
  
  //  resetMouse();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboard(unsigned char key, int x, int y) {  
  if (key == 'w') {
    movZ += 0.1f;
  }
  
  if (key == 's') {
    movZ -= 0.1f;
  }
  
  
  if (key == 'e') {
    movY += 0.1f;
  }
  
  
  if (key == 'q') {
    movY -= 0.1f;
  }
  
  if (key == 'a') {
    movX += 0.1f;
  }
  
  
  if (key == 'd') {
    movX -= 0.1f;
  }
  
  
}


int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowSize(WINDOW_X, WINDOW_Y);
  glutCreateWindow("Grid");
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboard);
  glutPassiveMotionFunc(passiveMotion);
  
  glutSetCursor (GLUT_CURSOR_NONE);
  resetMouse();
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST); 
  glEnable(GL_CULL_FACE);
  
  glClearColor(0.39,0.584,0.923,1.0);
  
  load_dae("/Users/NK/Desktop/court.dae");
  load_dae("/Users/NK/Desktop/box.dae");
  shaderProg = initShaders();
  
  glutMainLoop();
  
  return 0;	
}