#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include <set>
#include <btBulletDynamicsCommon.h>


class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
        static std::set<Light*> activeLights;
        static void addRigidBody(btRigidBody* pBody);
        static void removeRigidBody(btRigidBody* pBody);
        void updatePhysics(float pDelta);
	private:
	    Camera* _mainCamera;
        static btDiscreteDynamicsWorld* dynamicsWorld;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
