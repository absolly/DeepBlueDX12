#ifndef WORLD_H
#define WORLD_H

#include "mge/core/GameObject.hpp"
#include <set>

class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();
        static std::set<Light*> activeLights;
	private:
	    Camera* _mainCamera;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_H
