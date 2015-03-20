//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/mkOgModule.h>

#include <Og/Ogre/mkOgreModule.h>
#include <Og/Gorilla/mkGorillaWindow.h>

#include <Ui/Frame/mkUibox.h>
#include <Ui/Frame/mkFrame.h>

#include <Ui/mkUiWindow.h>
#include <Ui/mkUiLayout.h>

#include <Og/Input/mkOISInput.h>

#include <Sound/mkSoundManager.h>

#include <Ui/Widget/mkWidget.h>

#include <Og/Widget/mkWOgViewport.h>

#define CAP_FRAMERATE
#include <thread>

namespace mk
{
	OgWindow::OgWindow(OgModule* front, const string& name, int width, int height, bool fullScreen, User* user)
		: mOgModule(front)
		, mUiWindow(make_unique<UiWindow>(user))
		, mOgreWindow(front->ogreModule()->createWindow(mUiWindow.get(), name, width, height, fullScreen))
		, mGorillaWindow(make_unique<GorillaWindow>(mOgreWindow.get()))
	{
		mUiWindow->setup(mOgreWindow.get(), mGorillaWindow.get(), front->input());
		front->input()->initInput(mUiWindow.get(), mOgreWindow->handle());
		mOgreWindow->updateSize();
	}

	OgWindow::~OgWindow()
	{}


	bool OgWindow::nextFrame()
	{
		mGorillaWindow->nextFrame();
		bool pursue = mUiWindow->nextFrame();
		return pursue && !mOgreWindow->shutdown();
	}

	OgModule::OgModule(const string& pluginsPath, const string& resourcePath)
		: mOgreModule(make_unique<OgreModule>(pluginsPath, resourcePath))
		, mSoundManager(make_unique<SoundManager>())
		, mInput(make_unique<OISInput>())
	{}

	OgModule::~OgModule()
	{}

	void OgModule::initPrompt()
	{
		// Ogre
		mOgreModule->initStart();
		mOgreModule->configPrompt();
		mOgreModule->initEnd();

		// Sound
		mSoundManager->init();
	}

	void OgModule::initAuto()
	{
		// Ogre
		mOgreModule->initStart();
		mOgreModule->configAuto();
		mOgreModule->initEnd();

		// Sound
		mSoundManager->init();
	}

	bool OgModule::nextFrame()
	{
		mOgreModule->nextFrame();
		mInput->nextFrame();

		if(mOgreModule->contextActive())
			mSoundManager->threadUpdate();

		bool pursue = true;
		for(auto& window : mWindows)
			pursue &= window->nextFrame();

#ifdef CAP_FRAMERATE
		static Clock clock;
		double delta = 16.66666667 - (clock.step() / 1000.0);

		if(delta > 0.f)
			std::this_thread::sleep_for(std::chrono::milliseconds(int(delta)));
#endif

		return pursue;
	}

	OgWindow* OgModule::createWindow(const string& name, int width, int height, bool fullScreen, User* user)
	{
		unique_ptr<OgWindow> window = make_unique<OgWindow>(this, name, width, height, fullScreen, user);

		mWindows.emplace_back(std::move(window));
		return mWindows.back().get();
	}
}
