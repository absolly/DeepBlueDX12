#ifndef TestScene_H
#define TestScene_H

#include <mge/core/AbstractGame.hpp>
#include "mge/core/LuaParser.hpp"

class DebugHud;

class TestScene: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		TestScene();
		virtual ~TestScene();

        virtual void initialize();

	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display
        LuaParser* _parser;
        void _updateHud();

        TestScene(const TestScene&);
        TestScene& operator=(const TestScene&);

		void onTeapotCollisionWithPhysicsObject(btCollisionObject* collisionObject);
};

#endif // TestScene_H
