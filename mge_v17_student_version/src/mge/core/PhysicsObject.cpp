#include "PhysicsObject.hpp"
#include "mge/core/World.hpp"

PhysicsObject::PhysicsObject(std::string pName, glm::vec3 pPosition) : GameObject(pName, pPosition)
{
    //ctor
    //World::addRigidBody(rigidBody);
}

PhysicsObject::~PhysicsObject()
{
    //dtor
    World::removeRigidBody(rigidBody);
}

PhysicsObject::PhysicsObject(const PhysicsObject& other)
{
    //copy ctor
}

PhysicsObject& PhysicsObject::operator=(const PhysicsObject& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
