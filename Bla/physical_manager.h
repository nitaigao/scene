#ifndef PHYSICALMANAGER_H
#define PHYSICALMANAGER_H

class PhysicalManager
{
  
public:
  
  ~PhysicalManager() { };
  
  PhysicalManager() { };
  
  static PhysicalManager* manager();
  
  void destroy();
  
  void update(float delta_time);
  
  void reload();
  
private:
  
  PhysicalManager(const PhysicalManager & copy) { };
  PhysicalManager & operator = (const PhysicalManager & copy) { return *this; };
  
};

#endif