#ifndef IMAGE_H
#define IMAGE_H

#include <FreeImage.h>
#include <string>
#include <iostream>

class Texture {
  
public:

  static BYTE* loadImage(const std::string& path, unsigned int* width, unsigned int *height) {
    
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str(), 0);
    
    if(fif == FIF_UNKNOWN) {
      fif = FreeImage_GetFIFFromFilename(path.c_str());
    }
    
    if(fif == FIF_UNKNOWN) {
      std::cerr << "failed to determine image format for " << path << std::endl;
      return 0;
      
    }  
    FIBITMAP *dib = 0;
    if(FreeImage_FIFSupportsReading(fif)) {
      dib = FreeImage_Load(fif, path.c_str());
    }
    if(!dib) {
      std::cerr << "failed to read image data from " << path << std::endl;
      return 0;
      
    }  
    
    BYTE* bits = FreeImage_GetBits(dib);
    
    *width = FreeImage_GetWidth(dib);
    *height = FreeImage_GetHeight(dib);
    
    if(bits == 0 || width == 0 || height == 0) {
      std::cerr << "failed to get image dimensions for " << path << std::endl;
      return 0;
    }
    
    //  FreeImage_Unload(dib);
    
    return bits;
  }
};
#endif