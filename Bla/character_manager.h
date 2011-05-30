#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

class CharacterManager
{
  
public:
  
  ~CharacterManager() { };
  
  CharacterManager() { };
  
  static CharacterManager* manager();
  
  void destroy();
  
  void update(float delta_time);
    
private:
  
  CharacterManager(const CharacterManager & copy) { };
  CharacterManager & operator = (const CharacterManager & copy) { return *this; };
    
};

#endif