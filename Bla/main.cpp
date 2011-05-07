
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

float movZ;
float movY;
float movX;

float rotX;
float rotY;

glm::vec4 forward(0, 0, -1, 1.0f);
glm::vec4 left(-1, 0, 0, 1.0f);
glm::vec3 origin(0, -1, 0);

#include "json/reader.h"
#include "json/elements.h"
#include "IO.h"

using namespace json;

class Entity {
  
  Batch* batch_;
  glm::vec3 position_;
  
public:
  
  Entity(Batch* batch, glm::vec3 position)
  : batch_(batch)
  , position_(position) {
    
  }
  
  void render() {
        
    glm::mat4 projection = glm::perspective(75.0f, float(WINDOW_X) / float(WINDOW_Y), 0.5f, 100.f);
    
    glm::mat4 eyeRotationY = glm::rotate(projection, rotY, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 eyeRotationX = glm::rotate(eyeRotationY, rotX, glm::vec3(0.0f, 1.0f, 0.0f));   
        
    glm::vec3 newOrigin = origin + glm::vec3(forward) + glm::vec3(left);
    
    glm::mat4 eyeTranslation = glm::translate(eyeRotationX, origin);

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position_);
    
    batch_->render(translation, eyeTranslation);
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

class Scene {
  
  typedef std::vector<Entity*> EntityList;
  EntityList entities;
  
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
        
        const Object& skybox = (*i).element;
        
        String top = skybox["top"];
        String left = skybox["left"];
        String right = skybox["right"];
        String front = skybox["front"];
        String back = skybox["back"];
        
        const char* faces[5] = { top.Value().c_str(), left.Value().c_str(), right.Value().c_str(), front.Value().c_str(), back.Value().c_str() };
        
        GLenum cube[5] = { 
          GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
          GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
          GL_TEXTURE_CUBE_MAP_POSITIVE_X,
          GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
          GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
        };
        
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        

        GLbyte *pBytes; GLint iWidth, iHeight, iComponents; GLenum eFormat;
        
        for(int i = 0; i < 5; i++) { 
          pBytes = gltLoadTGABits(faces[i], &iWidth, &iHeight, &iComponents, &eFormat); 
          glTexImage2D(cube[i], 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
          free(pBytes); 
        } 
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
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

  //        batch->initShaders();
          
          Entity* entity = new Entity(batch, position);
          entities.push_back(entity);        
        }      
      }    
    }

  }
  
  void render() {
    
    for (EntityList::iterator i = entities.begin(); i != entities.end(); ++i) {
      (*i)->render();
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
    movY += 0.1f;
  }
  
  
  if (key == 'q') {
    movY -= 0.1f;
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
    
  scene.load("scene.json"); 
  
  glutMainLoop();
  
  return 0;	
}