#ifndef MOVEMENTSTATE_H
#define MOVEMENTSTATE_H

class MovementState
{
  
public:
  
  ~MovementState() { };
  
  MovementState() : x_delta_(0), y_delta_(0), forward_(false), back_(false), up_(false), down_(false), left_(false), right_(false) { };
  
  static MovementState* state();
  
  void destroy();
  
  void set_forward(bool moving);
  bool forward();  
  
  void set_back(bool moving);
  bool back();  
  
  void set_left(bool moving);
  bool left();  
  
  void set_right(bool moving);
  bool right();  
  
  void set_up(bool moving);
  bool up();
  
  void set_down(bool moving);
  bool down();
  
  void set_x_delta(float delta);
  float x_delta();
  
  void set_y_delta(float delta);
  float y_delta();
   
private:
  
  MovementState(const MovementState & copy) { };
  MovementState & operator = (const MovementState & copy) { return *this; };
    
private:
  
  bool forward_;
  bool back_;
  bool up_;
  bool down_;
  bool left_;
  bool right_;
  
  float x_delta_;
  float y_delta_;
  
};

#endif