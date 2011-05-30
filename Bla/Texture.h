#ifndef IMAGE_H
#define IMAGE_H

#include <FreeImage.h>
#include <string>
#include <iostream>

#include <OpenGL/OpenGL.h>

class Texture {
  
  GLuint textureId_;
  GLint target_;
  
public:
  
  Texture()
  : target_(GL_TEXTURE_2D) { };
  
  Texture(GLint target)
  : target_(target) { };
  
  void init(GLint mag, GLint min, GLint wrapS, GLint wrapT) {
    glGenTextures(1, &textureId_);
    glBindTexture(target_, textureId_);
    
    glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, mag); 
    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, min); 
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, wrapS); 
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, wrapT); 
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  
  void load(GLint target, const std::string& path) {
    unsigned int width, height = 0;
    BYTE* bits = 0;
    
    bits = Texture::loadImage(path.c_str(), &width, &height);  
    glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
  }
  
  void generateMipMap() {
    glGenerateMipmap(target_);
  }
  
  void bind() {
    glBindTexture(target_, textureId_);
  }

  static BYTE* loadImage(const std::string& path, unsigned int* width, unsigned int *height) {
    
    FreeImage_Initialise();
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
    
    FreeImage_DeInitialise();
    
    return bits;
  }
};
#endif