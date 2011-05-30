#ifndef INPUT_H
#define INPUT_H

class Input
{
  
public:
  
  ~Input() { };
  
  Input() { };
  
  static Input* system();
  
  void destroy();
  
  void update(float delta_time);
  
  void mouse_moved(int x, int y);
  
  void key_up(unsigned char key);
  
  void key_down(unsigned char key);
  
  void ignore_input_frame();
    
private:
  
  Input(const Input & copy) { };
  Input & operator = (const Input & copy) { return *this; };
  
private:
  
  bool ignore_input_frame_;
  
  int last_x;
  int last_y;
  
};

#endif