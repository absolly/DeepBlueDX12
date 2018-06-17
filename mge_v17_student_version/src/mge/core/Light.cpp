#include "Light.hpp"

//if(Light::activeLights != 0)
//    Light* Light::activeLights[255] = {nullptr};


Light::Light(lightType pType, std::string pName, glm::vec3 pPosition, glm::vec3 pColor, float pIntensity, glm::vec3 pFalloff, float pAngle)
    : GameObject(pName, pPosition), type(pType), intensity(pIntensity), falloff(pFalloff), angle(pAngle) {
    //ctor
    setColor(pColor);

}

Light::~Light() {
    //dtor
    //LightList::activeLights.erase(this);

}

Light::Light(const Light& other) {
    //copy ctor
}

Light& Light::operator=(const Light& rhs) {
    //assignment operator
    if (this == &rhs) return *this; // handle self assignment
    return *this;
}

void Light::setColor(glm::vec3 pColor) {
    _color = glm::normalize(pColor);

}

glm::vec3 Light::getColor() {
#ifdef API_OPENGL
    return _color;
#elif defined(API_DIRECTX)
	return _color / 255.0f;
#endif // API_OPENGL
}

void Light::setLightIntensity(float pIntensity)
{
	intensity = pIntensity;
}

void Light::setParent (GameObject* pGameObject) {

   // std::cout << "set parent of light called" << std::endl;
    if(pGameObject != NULL) {
        World::activeLights.insert(this);
        //std::cout << "added new light object to list" << std::endl;

       // std::cout << "Active Lights:" << std::endl;
        for(Light* light : World::activeLights) {
//std::cout << light->getName() << std::endl;
        }
    } else {
        World::activeLights.erase(this);
       // std::cout << "removed light object from list" << std::endl;

        //std::cout << "Active Lights:" << std::endl;
        for(Light* light : World::activeLights) {
          //  std::cout << light->getName() << std::endl;
        }
    }


    GameObject::setParent(pGameObject);
}
