//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGREFRONT_H_INCLUDED
#define MK_OGREFRONT_H_INCLUDED

/* Graphic Module */
#include <Object/mkTyped.h>
#include <Object/Util/mkTimer.h>
#include <Og/mkOgForward.h>

namespace mk
{
	MK_OG_EXPORT void createOgTestUi(Form& parent);

	class MK_OG_EXPORT OgWindow : public Object
	{
	public:
		OgWindow(OgModule& front, const string& name, int width, int height, bool fullScreen, User* user = nullptr);
		~OgWindow();

		bool nextFrame();

		OgModule& frontModule() { return mOgModule; }
		OgreWindow& ogreWindow() { return *mOgreWindow.get(); }
		GorillaWindow& gorillaWindow() { return *mGorillaWindow.get(); }
		UiWindow& uiWindow() { return *mUiWindow.get(); }

		static Type& cls() { static Type ty; return ty; }

	private:
		OgModule& mOgModule;
		unique_ptr<UiWindow> mUiWindow;
		unique_ptr<OgreWindow> mOgreWindow;
		unique_ptr<GorillaWindow> mGorillaWindow;

		unique_ptr<Uibox> mCursorBox;
	};

    class MK_OG_EXPORT OgModule : public Object
    {
    public:
        OgModule(const string& pluginsPath = "plugins.cfg", const string& resourcePath = "");
        ~OgModule();

		void initPrompt();
		void initAuto();

        bool nextFrame();

		OISInput& input() { return *mInput.get(); }

		OgWindow& createWindow(const string& name, int width, int height, bool fullScreen, User* user = nullptr);

		OgreModule& ogreModule() { return *mOgreModule.get(); }
#ifdef KIOG_SOUND
		SoundManager* soundManager() { return mSoundManager.get(); }
#endif

		static Type& cls() { static Type ty; return ty; }

    private:
		unique_ptr<OgreModule> mOgreModule;
		unique_ptr<OISInput> mInput;
#ifdef KIOG_SOUND
		unique_ptr<SoundManager> mSoundManager;
#endif

		std::vector<unique_ptr<OgWindow>> mWindows;
    };
}

#endif // MK_OGREFRONT_H_INCLUDED
