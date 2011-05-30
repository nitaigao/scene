#include "entity_system.h"
#include "renderer.h"
#include "Physics.h"
#include "input.h"
#include "clock.h"
#include "character_manager.h"
#include "physical_manager.h"

#include <GLUT/GLUT.h>
#include <ApplicationServices/ApplicationServices.h>

#define WINDOW_X 1280
#define WINDOW_Y 800

Clock timer;

void update() {
  float delta_time = timer.delta();
  CharacterManager::manager()->update(delta_time);
  Input::system()->update(delta_time);
  Physics::system()->update(delta_time);
}

void render() { 
  Renderer::system()->render();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void passiveMotion(int x, int y) {
  Input::system()->mouse_moved(x, y);
  
  static int WINDOW_MARGIN = 10;
  
  if (x <= (WINDOW_X / 2) - WINDOW_MARGIN || x >= (WINDOW_X / 2) + WINDOW_MARGIN) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(WINDOW_X / 2, y);
    glutPassiveMotionFunc(passiveMotion);
    Input::system()->ignore_input_frame();
  }
  
  if (y <= (WINDOW_Y / 2) - WINDOW_MARGIN || y >= (WINDOW_Y / 2) + WINDOW_MARGIN) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(x, WINDOW_Y / 2);
    glutPassiveMotionFunc(passiveMotion);
    Input::system()->ignore_input_frame();
  }
}

void keyboardUp(unsigned char key, int x, int y) {  
  Input::system()->key_up(key);
}

void keyboard(unsigned char key, int x, int y) {  
  Input::system()->key_down(key);
}

void init_system(int argc, char **argv, int width, int height) {
  CGSetLocalEventsSuppressionInterval(0.0);
  
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(width, height);
  glutCreateWindow("Grid");  
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutPassiveMotionFunc(passiveMotion);
  glutIdleFunc(update);
  
  glutSetCursor (GLUT_CURSOR_NONE);
  glutWarpPointer(width / 2, height / 2);  
}

void start_loop() {
  glutMainLoop();
}

int main(int argc, char **argv) {
  init_system(argc, argv, WINDOW_X, WINDOW_Y);
  
  CharacterManager::manager();
  EntitySystem::system();
  
  
  Renderer::system()->init(WINDOW_X, WINDOW_Y);
  Physics::system()->init();
  
  EntitySystem::system()->load("scene.json");
  
  
  Renderer::system()->reload();
  PhysicalManager::manager()->reload();
  
  
  start_loop();
  
  Renderer::system()->destroy();
  EntitySystem::system()->destroy();
  
  return 0;	
}