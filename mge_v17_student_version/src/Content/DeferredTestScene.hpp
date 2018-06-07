#ifndef TestScene_H
#define TestScene_H

#include <mge/core/AbstractGame.hpp>
#include "mge/core/LuaParser.hpp"
#include "Content\Hud\Hud.hpp"

class DebugHud;

class DeferredTestScene: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		DeferredTestScene();
		virtual ~DeferredTestScene();

        virtual void initialize(HINSTANCE pHinstance, HINSTANCE pPrevInstance, int pShowCmd);

	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		Hud* _hud;                   //hud display
        LuaParser* _parser;
        void _updateHud();

		DeferredTestScene(const DeferredTestScene&);
		DeferredTestScene& operator=(const DeferredTestScene&);
};

#endif // TestScene_H
