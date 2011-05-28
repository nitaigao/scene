#include "Physics.h"

#include <iostream>

#include "GLDebugDrawer.h"
static GLDebugDrawer debug_drawer;

void Physics::init() {
  configuration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(configuration);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver();
  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);
  world->setDebugDrawer(&debug_drawer);  
  world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawWireframe);
  world->setGravity(btVector3(0.0, -9.6f, 0.0f));  
}

void Physics::destroy() {
  delete world;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete configuration;
}

void Physics::update(float deltaTime) {
  world->stepSimulation(deltaTime);
}

void Physics::draw_debug() {
  world->debugDrawWorld();
}

void Physics::create_box() {
  btCollisionShape* shape = new btBoxShape(btVector3(3, 3, 3));
  
  btTransform transform;
  transform.setIdentity();
  
  btVector3 inertia(0.0f, 1.0f, 0.0f);
  btScalar mass(1.0f);
  shape->calculateLocalInertia(mass, inertia);
  
  btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
  btRigidBody* body = new btRigidBody(info);
  
  world->addRigidBody(body);
}

