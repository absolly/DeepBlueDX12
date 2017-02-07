#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <mge/core/GameObject.hpp>
#include <btBulletDynamicsCommon.h>


class PhysicsObject : public GameObject
{
    public:
        PhysicsObject(std::string pName = NULL, glm::vec3 pPosition = glm::vec3( 0.0f, 0.0f, 0.0f ));
        virtual ~PhysicsObject();
        PhysicsObject(const PhysicsObject& other);
        PhysicsObject& operator=(const PhysicsObject& other);
        btRigidBody* rigidBody = nullptr;

    protected:

    private:
};

#endif // PHYSICSOBJECT_H
