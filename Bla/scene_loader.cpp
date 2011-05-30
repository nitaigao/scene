#include "scene_loader.h"


#include "json/reader.h"
#include "json/elements.h"
using namespace json;

#include "IO.h"
#include "String.h"

#include "property.h"
#include "entity_system.h"


void SceneLoader::load(EntitySystem* entity_system) {
  string jsonData = IO::readFile(path_);
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
    
    const Array& entities =  (*i).element;
    for(Array::const_iterator entity = entities.Begin(); entity != entities.End(); ++entity) {
      
      const Array& behaviors = (*entity);
      for(Array::const_iterator behavior = behaviors.Begin(); behavior != behaviors.End(); ++behavior) {
        
        Property* property = entity_system->create_property();

        const Object& attributes = (*behavior);
        for(Object::const_iterator attribute = attributes.Begin(); attribute != attributes.End(); ++attribute) {
          
          try {
            std::string string_value = String(attributes[(*attribute).name]);          
            property->attr((*attribute).name, string_value);
          } catch (const Exception& e) { }
          
          try {
            Number number_value = Number(attributes[(*attribute).name]);
            property->attr((*attribute).name, number_value);            
          } catch (const Exception& e) { }
          
        }
        
        entity_system->add_property(property);                
      }
      
      entity_system->next_id();
    }
  }
}