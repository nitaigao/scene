
#include <vector>

#include <GLUT/GLUT.h>
#include <ApplicationServices/ApplicationServices.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4

#include "Batch.h"
#include "DAEImporter.h"
#include "io.h"

#include <sys/time.h>

#define WINDOW_X 1280
#define WINDOW_Y 800

struct timeval _tstart, _tend;

#include "json/reader.h"
#include "json/elements.h"

#include "IO.h"

#include "SkyBox.h"
#include "String.h"
#include "Scene.h"

using namespace json;

#include "Camera.h"

Scene scene;
Camera camera(WINDOW_X, WINDOW_Y);

void render() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  scene.render(camera.modelView(), camera.rotation());
    
  glutSwapBuffers();
  glutPostRedisplay();

}

float forwardVelocity;
float upVelocity;
float leftVelocity;

void update() {
  gettimeofday(&_tend,0);
  double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
  double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
  float deltaTime = float(t2-t1);
  _tstart = _tend;
  
  camera.update(forwardVelocity, leftVelocity, upVelocity, deltaTime);
}

int lastX = 0;
int lastY = 0;
bool firstInputPass = true;

float deg2rad(float degrees) {
  return degrees * (3.141f / 180.0f);
}

void passiveMotion(int x, int y) {
  if (!firstInputPass) {
    float xDelta = lastX - x;
    float yDelta = lastY - y;
    
    float xAngle = xDelta * 0.1f;
    float yAngle = yDelta * 0.1f;
    
    camera.rotateX(xAngle);
    camera.rotateY(yAngle);
  }
  
  lastX = x;
  lastY = y;
  
  firstInputPass = false;
  
  if (x <= (WINDOW_X / 2) - 10 || x >= (WINDOW_X / 2) + 10) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(WINDOW_X / 2, y);
    glutPassiveMotionFunc(passiveMotion);
    firstInputPass = true;
  }
  
  if (y <= (WINDOW_Y / 2) - 10 || y >= (WINDOW_Y / 2) + 10) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(x, WINDOW_Y / 2);
    glutPassiveMotionFunc(passiveMotion);
    firstInputPass = true;
  }

}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboardUp(unsigned char key, int x, int y) {  
  if (key == 'w') {
    forwardVelocity = 0;
  }
  
  if (key == 's') {    
    forwardVelocity = 0;
  }  
  
  if (key == 'e') {
    upVelocity = 0;;
  }
  
  
  if (key == 'q') {
    upVelocity = 0;
  }

  
  if (key == 'a') {
    leftVelocity = 0;
  }
  
  
  if (key == 'd') {
    leftVelocity = 0;
  }

}

void keyboard(unsigned char key, int x, int y) {  
  if (key == 'w') {
    forwardVelocity = -5.0f;
  }
  
  if (key == 's') {    
    forwardVelocity = 5.0f;
  }
  
  
  if (key == 'e') {
    upVelocity = 5.0f;
  }
  
  
  if (key == 'q') {
    upVelocity = -5.0f;
  }
  
  if (key == 'a') {
    leftVelocity = -5.0f;
  }
  
  
  if (key == 'd') {
    leftVelocity = 5.0f;
  }
}

int main(int argc, char **argv) {
  CGSetLocalEventsSuppressionInterval(0.0);
  
//	CGEventSourceSetLocalEventsSuppressionInterval(0, 0.0); 

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(WINDOW_X, WINDOW_Y);
  glutCreateWindow("Grid");
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutPassiveMotionFunc(passiveMotion);
  glutIdleFunc(update);
  
  glutSetCursor (GLUT_CURSOR_NONE);
  glutWarpPointer(WINDOW_X / 2, WINDOW_Y / 2);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  glClearColor(0.39,0.584,0.923,1.0);
  
  FreeImage_Initialise();
  
  camera = Camera(WINDOW_X, WINDOW_Y);

    
  scene.load("scene.json"); 
  
  glutMainLoop();
  
  FreeImage_DeInitialise();
  
  return 0;	
}