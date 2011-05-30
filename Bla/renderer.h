#ifndef RENDERER_H
#define RENDERER_H

#include <deque>

class Model;
class Camera;

class Renderer
{
  
public:
  
  ~Renderer() { };
  
  Renderer() { }
  
  static Renderer* system();
  
  void destroy();
  
  void reload();
  
  void render();
  
  void init(int width, int height);
  
private:
  
  Renderer(const Renderer & copy) { };
  Renderer & operator = (const Renderer & copy) { return *this; };
  
private:
  
  std::deque<Model*> models_;
  std::deque<Camera*> cameras_;
  int width_;
  int height_;
  
};

#endif
