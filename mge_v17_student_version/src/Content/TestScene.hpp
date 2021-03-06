#ifndef TestScene_H
#define TestScene_H

#include <mge/core/AbstractGame.hpp>
#include "mge/core/LuaParser.hpp"
#include "mge/core/LuaScriptParser.hpp"
#include "Content\Hud\Hud.hpp"
#include <vector>
#include "Content/Core/EventNoArgs.h"

class DebugHud;

class TestScene: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		TestScene();
		virtual ~TestScene();

        virtual void initialize(HINSTANCE pHinstance, HINSTANCE pPrevInstance, int pShowCmd);
		static EventNoArgs& resetEvent;

	protected:
		virtual void _update(float deltaTime);
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();
	private:
		Hud* _hud;                   //hud display
        LuaParser* _parser;
		LuaScriptParser * _scriptParser;
        void _updateHud();

        TestScene(const TestScene&);
        TestScene& operator=(const TestScene&);

		//void onRelicCollision(struct OnCollisionArgs onCollisionArgs);
		void onTempleDoorCollision(struct OnCollisionArgs onCollisionArgs);
		void onCollisionRemoveOther(struct OnCollisionArgs onCollisionArgs);
};

#endif // TestScene_H
