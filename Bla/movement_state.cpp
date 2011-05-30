#include "movement_state.h"

static MovementState* instance_ = 0;

MovementState* MovementState::state() {
  if (!instance_) {
    instance_ = new MovementState();
  }
  return instance_;
}

void MovementState::destroy() {
  delete instance_;
  instance_ = 0;
}

void MovementState::set_forward(bool moving) {
  forward_ = moving;
}

bool MovementState::forward() {
  return forward_;
}

void MovementState::set_left(bool moving) {
  left_ = moving;
}

bool MovementState::left() {
  return left_;
}

void MovementState::set_up(bool moving) {
  up_ = moving;
}

bool MovementState::up() {
  return up_;
}

void MovementState::set_down(bool moving) {
  down_ = moving;
}

bool MovementState::down() {
  return down_;
}

void MovementState::set_right(bool moving) {
  right_ = moving;
}

bool MovementState::right() {
  return right_;
}

void MovementState::set_back(bool moving) {
  back_ = moving;
}

bool MovementState::back() {
  return back_;
}

void MovementState::set_x_delta(float delta) {
  x_delta_ = delta;
}

float MovementState::x_delta() {
  return x_delta_;
}

void MovementState::set_y_delta(float delta) {
  y_delta_ = delta;
}

float MovementState::y_delta() {
  return y_delta_;
}
