//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGREMODULE_H_INCLUDED
#define MK_OGREMODULE_H_INCLUDED

/* mk */
#include <Object/mkTyped.h>
#include <Og/mkOgForward.h>
#include <Ui/mkRenderWindow.h>

#include <OgreWindowEventUtilities.h>

namespace mk
{
#ifdef D3D11_RTSHADERFIX
	class ShaderFixer;
#endif

	class MK_OG_EXPORT OgreWindow : public RenderWindow, public Ogre::WindowEventListener
	{
	public:
		OgreWindow(OgreModule* module, UiWindow* uiWindow, Ogre::RenderWindow* context);
		~OgreWindow();

		void updateSize();

		void windowResized(Ogre::RenderWindow* renderWindow);
		void windowFocusChange(Ogre::RenderWindow* renderWindow);
		void windowClosed(Ogre::RenderWindow* renderWindow);

		OgreModule* ogreModule() { return mModule; }
		Ogre::RenderWindow* context() { return mContext; }

	protected:
		OgreModule* mModule;
		UiWindow* mWindow;
		Ogre::RenderWindow* mContext;
	};

	class MK_OG_EXPORT _I_ OgreModule : public Object, public Typed<OgreModule>
	{
	public:
		OgreModule(const string& pluginsPath = "plugins.cfg", const string& resourcePath = "");
		~OgreModule();

		Ogre::Root* ogreRoot() { return mOgreRoot.get(); }
		bool contextActive() { return mContextActive; }

		void initStart();
		void configPrompt();
		void configAuto();
		void initEnd();

		void multiViewportWorkspace(const std::vector<OgViewport*>& viewports, Ogre::RenderTarget* target);

		void nextFrame();

		unique_ptr<OgreWindow> createWindow(UiWindow* window, const string& name, int width, int height, bool fullScreen);

		void focusChange() { mContextActive = !mContextActive; }

	private:
		void setupResources();
		void setupHiddenWindow();
		void setupFirstRenderer();

		void loadResources();

	private:
		unique_ptr<Ogre::Root> mOgreRoot;
		string mResourcePath;
		bool mContextActive;
	};
}

#endif // MK_OGREMODULE_H_INCLUDED
