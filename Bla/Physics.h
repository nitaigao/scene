#ifndef PHYSICS_H
#define PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include "glm/glm.hpp"
#include <string>

class Camera;

class Physics {
  
  btDynamicsWorld* world;
  
  btDefaultCollisionConfiguration* configuration;
  btCollisionDispatcher* dispatcher;
  btDbvtBroadphase* broadphase;
  btSequentialImpulseConstraintSolver* solver;
  
public:
  
  Physics() : world(0) { }
  
  void init();
  
  static Physics* system();
  
  void destroy();
  
  void update(float deltaTime);
  
  void draw_debug(Camera* camera);
  
  void create_box(const glm::vec3& position, float mass);
  
  void create_mesh(const std::string& path, unsigned int label);
  
};

#endif