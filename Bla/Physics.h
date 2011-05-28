#ifndef PHYSICS_H
#define PHYSICS_H

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

class Physics {
  
  btDynamicsWorld* world;
  
  btDefaultCollisionConfiguration* configuration;
  btCollisionDispatcher* dispatcher;
  btDbvtBroadphase* broadphase;
  btSequentialImpulseConstraintSolver* solver;
  
public:
  
  Physics() : world(0) 
  {
    
    
  }
  
  void init();
  
  void destroy();
  
  void update(float deltaTime);
  
  void draw_debug();
  
  void create_box();
  
};

#endif