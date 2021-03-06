#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif
#include <windows.h>
#include <iostream>


#include "Content/DeferredTestScene.hpp"


extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
/**
 * Main entry point for the Micro Engine.

 * Design goals:
 * - correct division of OpenGL into appropriate wrapper classes
 * - simple design
 * - each class should know as little as possible about other classes and non related parts of OpenGL
 * - the engine must allow different objects with different transforms, meshes, materials (shaders) etc
 * - consistent coding conventions
 * - reusable and extendable core set of classes which should require no modification
 *   (in other words it is possible to have a working "empty" example)
 *
 * All documentation is contained within the HEADER files, not the CPP files in possible.
 *
 */

int main() {
	WinMain(GetModuleHandle(NULL), NULL, NULL, 1);
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	std::cout << "Starting Game" << std::endl;

	//printf("Starting Game!");

	AbstractGame* game = new DeferredTestScene();
	game->initialize(hInstance, hPrevInstance, nShowCmd);
	game->run();

	//delete game;

	return 0;
}




