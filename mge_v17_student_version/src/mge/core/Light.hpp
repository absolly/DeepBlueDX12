#ifndef LIGHT_H
#define LIGHT_H

#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"

class Light : public GameObject {
    public:
        enum lightType{
            POINT,
            DIRECTIONAL,
            SPOT
        };
        enum lightFalloff{
            CONSTANT,
            LINEAR,
            QUADRATIC
        };

        Light(lightType pType, std::string pName, glm::vec3 pPosition, glm::vec3 pColor = glm::vec3(1,1,1), float pIntensity = 1.0f, lightFalloff pFalloff = lightFalloff::QUADRATIC, float pAngle = 45.0f);
        virtual ~Light();
        Light(const Light& other);
        Light& operator=(const Light& other);

        lightType type;
        lightFalloff falloff;
        void setColor(glm::vec3 pColor);
        glm::vec3 getColor();
        float intensity;
        float angle;
        void setParent (GameObject* pGameObject);

    protected:
    private:

        glm::vec3 _color;

};



#endif // LIGHT_H
