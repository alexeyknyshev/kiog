//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Input/mkOISInput.h>

#include <sstream>
#include <iostream>
namespace mk
{
    MouseButton convertOISButton(int buttonID)
	{
	   switch (buttonID)
	   {
	   case OIS::MB_Left:
		   return LEFT_BUTTON;
	   case OIS::MB_Right:
		   return RIGHT_BUTTON;
	   case OIS::MB_Middle:
		   return MIDDLE_BUTTON;
	   default:
		   return LEFT_BUTTON;
	   }
	}

    OISInput::OISInput()
        : mInputManager(0)
        , mMouse(0)
        , mKeyboard(0)
		, mDispatcher(0)
		, mShutdownRequested(false)
    {}

    OISInput::~OISInput()
    {
        destroyInput();
    }

	void OISInput::initInput(InputDispatcher& dispatcher, size_t windowHnd)
	{
		mDispatcher = &dispatcher;
		setupInput(windowHnd);
	}

    bool OISInput::nextFrame()
    {
        mMouse->capture();
        mKeyboard->capture();

		return !mShutdownRequested;
	}

	void OISInput::resize(size_t width, size_t height)
	{
        const OIS::MouseState& mouseState = mMouse->getMouseState();
        mouseState.width = width;
        mouseState.height = height;
	}
	
    void OISInput::setupInput(size_t windowHnd)
    {
        std::ostringstream windowHndStr;
        OIS::ParamList paramList;

        windowHndStr << (unsigned int)windowHnd;
        paramList.insert(std::make_pair(string("WINDOW"), windowHndStr.str()));
        mInputManager = OIS::InputManager::createInputSystem(paramList);

        mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
        mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));

        mKeyboard->setEventCallback(this);
        mMouse->setEventCallback(this);
    }

    void OISInput::destroyInput()
    {
        if(mInputManager != 0)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }

	bool OISInput::mouseMoved(const OIS::MouseEvent &arg)
	{
		mDispatcher->dispatchMouseMoved(float(arg.state.X.abs), float(arg.state.Y.abs), float(arg.state.X.rel), float(arg.state.Y.rel));

		if(arg.state.Z.rel != 0)
			mDispatcher->dispatchMouseWheeled(float(arg.state.X.abs), float(arg.state.Y.abs), float(arg.state.Z.rel));

		return true;
	}

	bool OISInput::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		if(mPressed[id])
			this->mouseReleased(arg, id);

		mPressed[id] = true;
		mDispatcher->dispatchMousePressed(float(arg.state.X.abs), float(arg.state.Y.abs), convertOISButton(id));
		return true;
	}

	bool OISInput::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
	{
		if(!mPressed[id])
			return true;

		mPressed[id] = false;
		mDispatcher->dispatchMouseReleased(float(arg.state.X.abs), float(arg.state.Y.abs), convertOISButton(id));
		return true;
	}

	char OISInput::getChar(const OIS::KeyEvent &arg)
	{
		switch(arg.key)
		{
		case KC_NUMPAD0: return '0';
		case KC_NUMPAD1: return '1';
		case KC_NUMPAD2: return '2';
		case KC_NUMPAD3: return '3';
		case KC_NUMPAD4: return '4';
		case KC_NUMPAD5: return '5';
		case KC_NUMPAD6: return '6';
		case KC_NUMPAD7: return '7';
		case KC_NUMPAD8: return '8';
		case KC_NUMPAD9: return '9';
		}

		return arg.text;
	}

	bool OISInput::keyPressed(const OIS::KeyEvent &arg)
	{
		if(arg.key == OIS::KC_ESCAPE)
			mShutdownRequested = true;

		mDispatcher->dispatchKeyPressed(static_cast<KeyCode>(arg.key), getChar(arg));
		return true;
	}

	bool OISInput::keyReleased(const OIS::KeyEvent &arg)
	{
		mDispatcher->dispatchKeyReleased(static_cast<KeyCode>(arg.key), getChar(arg));
		return true;
	}
}
