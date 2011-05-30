#include "input.h"
#include "movement_state.h"
#include <iostream>

static Input* instance_ = 0;

Input* Input::system() {
  if (!instance_) {
    instance_ = new Input();
  }
  return instance_;
}

void Input::destroy() {
  delete instance_;
  instance_ = 0;
}

void Input::update(float delta_time) {
  MovementState::state()->set_x_delta(0);
  MovementState::state()->set_y_delta(0);
}

void Input::ignore_input_frame() {
  ignore_input_frame_ = true;
}

void Input::mouse_moved(int x, int y) {
  if (!ignore_input_frame_) {
    static float INPUT_FILTER = 0.1f;
    float x_delta = (last_x - x) * INPUT_FILTER;
    float y_delta = (last_y - y) * INPUT_FILTER;
  
    MovementState::state()->set_x_delta(x_delta);
    MovementState::state()->set_y_delta(y_delta);
  }
  
  last_x = x;
  last_y = y;
  ignore_input_frame_ = false;
}

void Input::key_up(unsigned char key) {
  
  if (key == 'w') {
    MovementState::state()->set_forward(false);  
  }
  
  if (key == 's') {    
    MovementState::state()->set_back(false);  
  }  
  
  if (key == 'e') {
    MovementState::state()->set_up(false);  
  }
  
  
  if (key == 'q') {
    MovementState::state()->set_down(false);  
  }

  
  if (key == 'a') {
    MovementState::state()->set_left(false);  
  }
  
  
  if (key == 'd') {
    MovementState::state()->set_right(false);  
  }

}

void Input::key_down(unsigned char key) {
  if (key == 'w') {
    MovementState::state()->set_forward(true);  
  }
  
  if (key == 's') {    
    MovementState::state()->set_back(true);  
  }
  
  if (key == 'e') {
    MovementState::state()->set_up(true);  
  }
  
  if (key == 'q') {
    MovementState::state()->set_down(true);  
  }
  
  if (key == 'a') {
    MovementState::state()->set_left(true);  
  }
    
  if (key == 'd') {
    MovementState::state()->set_right(true);  
  }
}

