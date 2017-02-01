#include "mge/core/World.hpp"

using namespace std;

set<Light*> World::activeLights = set<Light*>();

World::World():GameObject("root"), _mainCamera(0)
{
	//ctor
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}
