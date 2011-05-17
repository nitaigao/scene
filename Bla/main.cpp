
#include <vector>

#include <GLUT/GLUT.h>
#include <ApplicationServices/ApplicationServices.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4

#include "Batch.h"
#include "DAEImporter.h"
#include "io.h"

#include <sys/time.h>

#define WINDOW_X 1280
#define WINDOW_Y 800

struct timeval _tstart, _tend;

float rotX;
float rotY;

glm::vec4 forward(0, 0, -1, 1.0f);
glm::vec4 left(-1, 0, 0, 1.0f);
glm::vec4 up(0, 1, 0, 1.0f);
glm::vec3 origin(0, -1, 0);

#include "json/reader.h"
#include "json/elements.h"
#include "IO.h"
#include <FreeImage.h>

using namespace json;

class Entity {
  
  Batch* batch_;
  glm::vec3 position_;
  
public:
  
  Entity(Batch* batch, glm::vec3 position)
  : batch_(batch)
  , position_(position) {
    
  }
  
  void render(const glm::mat4& projection) {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
    
    batch_->render(translation, projection);
  }
  
};

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


BYTE* loadImage(const std::string& path, unsigned int* width, unsigned int *height) {
	
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

class SkyBox {
  
  std::string top_;
  std::string bottom_;
  std::string left_;
  std::string right_;
  std::string front_;
  std::string back_;
  
  Batch* batch_;
  
  GLuint textureId_;
    
public:
  
  SkyBox() : batch_(0) { };
  
  void setTop(const std::string& top) { top_ = top; };
  void setBottom(const std::string& bottom) { bottom_ = bottom; };
  void setLeft(const std::string& left) { left_ = left; };
  void setRight(const std::string& right) { right_ = right; };
  void setFront(const std::string& front) { front_ = front; };
  void setBack(const std::string& back) { back_ = back; };
  
  void load() {
    
    static const int CUBE_SIZE = 6;
    
    const char* faces[CUBE_SIZE] = { top_.c_str(), bottom_.c_str(), left_.c_str(), right_.c_str(), front_.c_str(), back_.c_str() };
    
    GLenum cube[CUBE_SIZE] = { 
      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
      GL_TEXTURE_CUBE_MAP_POSITIVE_X,
      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
      GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };

    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for(int i = 0; i < CUBE_SIZE; i++) { 
      unsigned int width, height;      
      BYTE* bits = loadImage(faces[i], &width, &height);
      glTexImage2D(cube[i], 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);      
    } 
    
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    batch_ = DAEImporter::load_dae("skybox.dae");
    batch_->initShaders("skybox");
  }
  
  void render(const glm::mat4& projection) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    batch_->render(glm::mat4(1.0f), projection);
  }
};

class Scene {
  
  typedef std::vector<Entity*> EntityList;
  EntityList entities;
  SkyBox skybox;
  
public:
  
  void load(const std::string& path) {
    
    string jsonData = IO::readFile(path);
    jsonData = jsonData.replace("\xff", "");
    std::stringstream stream(jsonData);
    
    Object doc;
    
    try {
      Reader::Read(doc, stream);
    }
    catch (Exception e) {
      std::clog << e.what() << std::endl;
      throw e;
    }
    
    for (Object::const_iterator i = doc.Begin(); i != doc.End(); ++i) {
      
      string name = (*i).name;

      if (name == "skybox") {
        
        const Object& sb = (*i).element;
        
        skybox.setTop(String(sb["top"]));
        skybox.setBottom(String(sb["bottom"]));
        skybox.setLeft(String(sb["left"]));
        skybox.setRight(String(sb["right"]));
        skybox.setFront(String(sb["front"]));
        skybox.setBack(String(sb["back"]));
        
        skybox.load();
      }
      
      if (name == "entities") {
        
        const Array& jentities =  (*i).element;
        for(Array::const_iterator jentity = jentities.Begin(); jentity != jentities.End(); ++jentity) {
         
          std::string model = String((*jentity)["model"]);
          Number positionX = Number((*jentity)["position"]["x"]);
          Number positionY = Number((*jentity)["position"]["y"]);
          Number positionZ = Number((*jentity)["position"]["z"]);
          glm::vec3 position(positionX, positionY, positionZ);

          Batch* batch = DAEImporter::load_dae(model);
          
          Entity* entity = new Entity(batch, position);
          entities.push_back(entity);        
        }      
      }    
    }

  }
  
  void render() {
    glm::mat4 projection = glm::perspective(75.0f, float(WINDOW_X) / float(WINDOW_Y), 0.5f, 200.0f);
    
    glm::mat4 eyeRotationY = glm::rotate(projection, rotY, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 eyeRotationX = glm::rotate(eyeRotationY, rotX, glm::vec3(0.0f, 1.0f, 0.0f));           
    glm::mat4 eyeTranslation = glm::translate(eyeRotationX, origin);

    skybox.render(eyeRotationX);   
    
    for (EntityList::iterator i = entities.begin(); i != entities.end(); ++i) {
      (*i)->render(eyeTranslation);
    }
  };
  
};

Scene scene;

void render() { 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  scene.render();
    
  glutSwapBuffers();
  glutPostRedisplay();
  
}

float forwardVelocity;
float upVelocity;
float leftVelocity;

void update() {
  gettimeofday(&_tend,0);
  double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
  double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
  float deltaTime = float(t2-t1);
  _tstart = _tend;
  
  origin.x += (forward.x * forwardVelocity) * deltaTime;
  origin.y += (forward.y * forwardVelocity) * deltaTime;
  origin.z += (forward.z * forwardVelocity) * deltaTime;
  
  origin.x += (left.x * leftVelocity) * deltaTime;
  origin.y += (left.y * leftVelocity) * deltaTime;
  origin.z += (left.z * leftVelocity) * deltaTime;
  
  origin.x += (up.x * upVelocity) * deltaTime;
  origin.y += (up.y * upVelocity) * deltaTime;
  origin.z += (up.z * upVelocity) * deltaTime;
}

int lastX = 0;
int lastY = 0;
bool firstInputPass = true;

float deg2rad(float degrees) {
  return degrees * (3.141f / 180.0f);
}

void passiveMotion(int x, int y) {
  if (!firstInputPass) {
    float xDelta = lastX - x;
    float yDelta = lastY - y;
    
    float xAngle = xDelta * 0.1f;
    float yAngle = yDelta * 0.1f;

    rotX -= xAngle;
    rotY -= yAngle;
    
    glm::mat4 cameraRotated = glm::rotate(glm::mat4(1.0), -xAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    forward = forward * cameraRotated;
    left = left * cameraRotated;
  }
  
  lastX = x;
  lastY = y;
  
  firstInputPass = false;
  
  if (x <= (WINDOW_X / 2) - 10 || x >= (WINDOW_X / 2) + 10) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(WINDOW_X / 2, y);
    glutPassiveMotionFunc(passiveMotion);
    firstInputPass = true;
  }
  
  if (y <= (WINDOW_Y / 2) - 10 || y >= (WINDOW_Y / 2) + 10) {
    glutPassiveMotionFunc(0);
    glutWarpPointer(x, WINDOW_Y / 2);
    glutPassiveMotionFunc(passiveMotion);
    firstInputPass = true;
  }

}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void keyboardUp(unsigned char key, int x, int y) {  
  if (key == 'w') {
    forwardVelocity = 0;
  }
  
  if (key == 's') {    
    forwardVelocity = 0;
  }  
  
  if (key == 'e') {
    upVelocity = 0;;
  }
  
  
  if (key == 'q') {
    upVelocity = 0;
  }

  
  if (key == 'a') {
    leftVelocity = 0;
  }
  
  
  if (key == 'd') {
    leftVelocity = 0;
  }

}

void keyboard(unsigned char key, int x, int y) {  
  if (key == 'w') {
    forwardVelocity = -5.0f;
  }
  
  if (key == 's') {    
    forwardVelocity = 5.0f;
  }
  
  
  if (key == 'e') {
    upVelocity = 5.0f;
  }
  
  
  if (key == 'q') {
    upVelocity = -5.0f;
  }
  
  if (key == 'a') {
    leftVelocity = -5.0f;
  }
  
  
  if (key == 'd') {
    leftVelocity = 5.0f;
  }
}

int main(int argc, char **argv) {
  

  CGSetLocalEventsSuppressionInterval(0.0);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(WINDOW_X, WINDOW_Y);
  glutCreateWindow("Grid");
  
  glutReshapeFunc(reshape);
  glutDisplayFunc(render);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutPassiveMotionFunc(passiveMotion);
  glutIdleFunc(update);
  
  glutSetCursor (GLUT_CURSOR_NONE);
  glutWarpPointer(WINDOW_X / 2, WINDOW_Y / 2);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  glClearColor(0.39,0.584,0.923,1.0);
  
  FreeImage_Initialise();
    
  scene.load("scene.json"); 
  
  glutMainLoop();
  
  FreeImage_DeInitialise();
  
  return 0;	
}