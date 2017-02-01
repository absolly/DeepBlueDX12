#ifndef MGEDEMO_H
#define MGEDEMO_H

#include <mge/core/AbstractGame.hpp>
#include "mge/core/LuaParser.hpp"

class DebugHud;

class MGEDemo: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		MGEDemo();
		virtual ~MGEDemo();

        virtual void initialize();

	protected:
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display
        LuaParser* _parser;
        void _updateHud();

        MGEDemo(const MGEDemo&);
        MGEDemo& operator=(const MGEDemo&);
};

#endif // MGEDEMO_H
