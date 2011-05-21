#ifndef STRING_H
#define STRING_H

class string : public std::string {
  
public:
  
  string (const std::string& other) 
  : std::string(other) {
    
  }
  
  std::string replace(const std::string &from, const std::string & to) {
    std::string snew = *this;
    std::string::size_type pos = std::string::npos;
    
    if((pos = snew.find(from)) != std::string::npos) {
      return snew.replace(pos, from.length(), to);
    }
    
    return snew;
  }  
  
  
  bool operator== (const string& other) const {
    return std::string(*this).compare(other) == 0;
  }
  
  
};

#endif