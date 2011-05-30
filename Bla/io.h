#ifndef IO_H
#define IO_H

#include <string>

class IO
{
  
public:
  
  ~IO() { };
  
  IO() { }
  
  static std::string readFile(const std::string& filename);
  
  
private:
  
  IO(const IO & copy) { };
  IO & operator = (const IO & copy) { return *this; };
  
};

#endif
