
#include <vector>

#include <GLUT/GLUT.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4

#include "Batch.h"
#include "DAEImporter.h"

#define WINDOW_X 800
#define WINDOW_Y 600

struct timeval _tstart, _tend;

float movZ;
float movY;
float movX;

float rotX;
float rotY;

typedef std::vector<Batch*> BatchList;
BatchList batches;

#include <libjson/libjson.h>
#include <libjson/Source/JSONDefs.h>

class Scene {
  
public:
  
  void load(const std::string& path) {
    
    

//    JSONNode n = libjson::parse(json);
    
    
    
  }
  
  void render() {
    
  };
  
};

Scene scene;

void render() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glm::mat4 projection = glm::perspective(75.0f, float(WINDOW_X) / float(WINDOW_Y), 0.5f, 100.f);
  
  glm::mat4 eyeRotationY = glm::rotate(glm::mat4(1.0f), rotY, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 eyeRotationX = glm::rotate(eyeRotationY, rotX, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 eyeTranslation = glm::translate(eyeRotationX, glm::vec3(0.0f + movX, -2.0f + movY, -2.0f + movZ));
  
  
  for (BatchList::iterator i = batches.begin(); i != batches.end(); ++i) {
    (*i)->render(eyeTranslation, projection);
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
  glutWarpPointer(WINDOW_X / 2, WINDOW_Y / 2);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST); 
  glEnable(GL_CULL_FACE);
  
  glClearColor(0.39,0.584,0.923,1.0);
  
  scene.load("scene.json");
  
  
//  Batch* batch1 = DAEImporter::load_dae("/Users/NK/Desktop/court.dae");
//  batches.push_back(batch1);
//  
//  Batch* batch2 = DAEImporter::load_dae("/Users/NK/Desktop/box.dae");
//  batches.push_back(batch2);
  
  glutMainLoop();
  
  return 0;	
}