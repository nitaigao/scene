#ifndef IO_H
#define IO_H

class IO {
  
public:
  
  static std::string readFile(const std::string& filename) {
    std::ifstream ifs (filename.c_str(), std::ifstream::in );
    std::stringstream output;
    
    while (ifs.good()) {
      output << (char) ifs.get();
    }
    return output.str();
  }
};

#endif