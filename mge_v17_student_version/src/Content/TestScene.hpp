#ifndef TestScene_H
#define TestScene_H

#include <mge/core/AbstractGame.hpp>
#include "mge/core/LuaParser.hpp"
#include "Content\Hud\Hud.hpp"
#include <vector>

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
		Hud* _hud;                   //hud display
        LuaParser* _parser;
        void _updateHud();

        TestScene(const TestScene&);
        TestScene& operator=(const TestScene&);

		void onCollisionRemoveSelf(struct OnCollisionArgs onCollisionArgs);
		void onCollisionRemoveOther(struct OnCollisionArgs onCollisionArgs);
};

#endif // TestScene_H
