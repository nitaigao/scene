#include "Physics.h"
#include "Camera.h"

#include <iostream>
#include "DAEImporter.h"

#include "GLDebugDrawer.h"
static GLDebugDrawer debug_drawer;

static Physics* instance_ = 0;

Physics* Physics::system() {
  if (!instance_) {
    instance_ = new Physics();
  }
  return instance_;
}

void Physics::destroy() {
  delete world;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete configuration;

  delete instance_;
  instance_ = 0;
}

void Physics::init() {
  configuration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(configuration);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver();
  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);
  debug_drawer.init();
  world->setDebugDrawer(&debug_drawer);
  world->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
  world->setGravity(btVector3(0.0, -9.6f, 0.0f));  
}

void Physics::update(float deltaTime) {
  world->stepSimulation(deltaTime);
}

void Physics::draw_debug(Camera* camera) {
  debug_drawer.set_camera(camera);
  world->debugDrawWorld();
}

void Physics::create_box(const glm::vec3& position, float mass) {
  btCollisionShape* shape = new btBoxShape(btVector3(1, 1, 1));
  
  btTransform transform;
  transform.setIdentity();
  
  btVector3 inertia(0.0f, 1.0f, 0.0f);
  shape->calculateLocalInertia(mass, inertia);
  
  btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo info(mass, motion_state, shape, inertia);
  btRigidBody* body = new btRigidBody(info);
  body->translate(btVector3(position.x, position.y, position.z));
  
  world->addRigidBody(body);
}

void Physics::create_mesh(const std::string& path, unsigned int label) {
  DAEFile dae;
  dae.load(path);
}

