
#include <vector>

#include <GLUT/GLUT.h>
#include <ApplicationServices/ApplicationServices.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4

#include "Batch.h"
#include "DAEImporter.h"
#include "io.h"

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

std::string replace(const std::string& str,const std::string &from,const std::string & to) {
  std::string snew = str;
  std::string::size_type pos = std::string::npos;
  
  if((pos = snew.find(from)) != std::string::npos) {
    return snew.replace(pos, from.length(), to);
  }

  return snew;
}

class Scene {
  
  typedef std::vector<Entity*> EntityList;
  EntityList entities;
  
public:
  
  void load(const std::string& path) {
    
    std::string jsonData = IO::readFile(path);
    jsonData = replace(jsonData, "\xff", "");
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

void update() {
  //  gettimeofday(&_tend,0);
  //  double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
  //  double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
  //  float deltaTime = float(t2-t1);
  //  _tstart = _tend;
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

void keyboard(unsigned char key, int x, int y) {  
  if (key == 'w') {
    origin.x += forward.x * -0.1f;
    origin.y += forward.y * -0.1f;
    origin.z += forward.z * -0.1f;
  }
  
  if (key == 's') {
    origin.x += forward.x * 0.1f;
    origin.y += forward.y * 0.1f;
    origin.z += forward.z * 0.1f;
  }
  
  
  if (key == 'e') {
    movY += 0.1f;
  }
  
  
  if (key == 'q') {
    movY -= 0.1f;
  }
  
  if (key == 'a') {
    origin.x += left.x * -0.1f;
    origin.y += left.y * -0.1f;
    origin.z += left.z * -0.1f;
  }
  
  
  if (key == 'd') {
    origin.x += left.x * 0.1f;
    origin.y += left.y * 0.1f;
    origin.z += left.z * 0.1f;
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
  glutPassiveMotionFunc(passiveMotion);
  
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