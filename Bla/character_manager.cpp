#include "character_manager.h"
#include <deque>
#include "property.h"
#include "entity_system.h"
#include "movement_state.h"

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


static const std::string CHARACTER_TYPE = "character";
static const std::string SPATIAL_TYPE = "spatial";
static const float MOVEMENT_DISTANCE = 8.0f;

static CharacterManager* instance_ = 0;

CharacterManager* CharacterManager::manager() {
  if (!instance_) {
    instance_ = new CharacterManager();
  }
  return instance_;
}

void CharacterManager::destroy() {
  delete instance_;
  instance_ = 0;
}

glm::vec4 adjust_directions(const glm::vec4& direction, float angle) {
  glm::vec3 up(0, 1, 0);    
  glm::mat4 left_matrix = glm::rotate(glm::mat4(1.0), angle, up);
  return direction * left_matrix;  
}

void update_origin(Property* spatial, Property* character, float delta_time) {
  glm::vec4 origin = spatial->attrv4("origin");
  float speed = MOVEMENT_DISTANCE * delta_time;
  
  if (MovementState::state()->forward()) {      
    glm::vec4 forward = character->attrv4("forward");      
    origin += forward * speed;
  }
  
  if (MovementState::state()->back()) {
    glm::vec4 forward = character->attrv4("forward");
    origin -= forward * speed;
  }
  
  if (MovementState::state()->left()) {
    glm::vec4 left = character->attrv4("left");
    origin += left * speed;
  }
  
  if (MovementState::state()->right()) {
    glm::vec4 left = character->attrv4("left");
    origin -= left * speed;
  }
  
  if (MovementState::state()->up()) {
    glm::vec4 up = character->attrv4("up");
    origin += up * speed;
  }
  
  if (MovementState::state()->down()) {
    glm::vec4 up = character->attrv4("up");
    origin -= up * speed;
  }
  
  spatial->attr("origin", origin);
}

void update_rotation(Property* spatial) {
  float new_ry = spatial->attrf("ry") + MovementState::state()->y_delta();;
  spatial->attr("ry", new_ry);
  
  float new_rx = spatial->attrf("rx") - MovementState::state()->x_delta();
  spatial->attr("rx", new_rx); 
}

void update_direction(Property* character, float x_delta) {
  glm::vec4 new_left = adjust_directions(character->attrv4("left"), -x_delta);
  character->attr("left", new_left);          
  
  glm::vec4 new_forward = adjust_directions(character->attrv4("forward"), -x_delta);
  character->attr("forward", new_forward);          
}

void CharacterManager::update(float delta_time) {
  std::deque<Property*> characters = EntitySystem::system()->query(CHARACTER_TYPE);
  for (std::deque<Property*>::iterator i = characters.begin(); i != characters.end(); ++i) {
    Property* spatial = EntitySystem::system()->query(SPATIAL_TYPE, (*i)->label());
    update_rotation(spatial);
    update_direction((*i), MovementState::state()->x_delta());    
    update_origin(spatial, (*i), delta_time);
  }
}