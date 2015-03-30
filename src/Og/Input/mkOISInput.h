//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_INPUTWINDOW_H_INCLUDED
#define MK_INPUTWINDOW_H_INCLUDED

/* mk */
#include <Og/mkOgForward.h>
#include <Ui/Input/mkInputDispatcher.h>

/* OIS headers */
#include <OIS.h>

namespace mk
{
    class MK_OG_EXPORT OISInput : public InputWindow, public OIS::KeyListener, public OIS::MouseListener
    {
	public:
        OISInput();
        virtual ~OISInput();

		void initInput(InputDispatcher* dispatcher, size_t windowHnd);
		void setupInput(size_t windowHnd);
		void destroyInput();

        bool nextFrame();
		void resize(size_t width, size_t height);

		//virtual void updateSize() = 0;

		// OISInput::InputListener
        bool mouseMoved(const OIS::MouseEvent &arg);
        bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
        bool keyPressed(const OIS::KeyEvent &arg);
        bool keyReleased(const OIS::KeyEvent &arg);

		char getChar(const OIS::KeyEvent &arg);

    protected:
        /** Input Library components **/
        OIS::InputManager* mInputManager;
        OIS::Mouse* mMouse;
        OIS::Keyboard* mKeyboard;

		InputDispatcher* mDispatcher;

		std::map<OIS::MouseButtonID, bool> mPressed;
        bool mShutdownRequested;
    };

}

#endif // MK_INPUTWINDOW_H_INCLUDED
