#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include <set>
#include <btBulletDynamicsCommon.h>
#include "mge/core/Light.hpp"
class Camera;

//make sure the lights are in the correct order. this matters because we send the lights to the gpu in one array and define how many of each we have.
struct lightComp {
	bool operator() ( Light* const lhs, Light* const rhs) const {
		if (lhs->type != rhs->type)
			return lhs->type < rhs->type;
		else
			return true;
	}
};

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		static Camera* getMainCamera();
        static std::set<Light*, lightComp> activeLights;
        static void removeRigidBody(btRigidBody* pBody);
        void updatePhysics(float pDelta);
		void debugDraw();
		static class PhysicsWorld* physics;
	private:
	    static Camera* _mainCamera;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
