//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_GORILLAWINDOW_H
#define MK_GORILLAWINDOW_H

/* mk Og */
#include <Og/mkOgForward.h>
#include <Ui/Frame/mkInk.h>

#include <Ogre.h>

#include <Og/Gorilla/Gorilla.h>
#include <Og/Gorilla/RectPacking/Rect.h>
#include <Og/Gorilla/RectPacking/GuillotineBinPack.h>

#include <memory>

namespace mk
{
	struct ImageRect : public BPRect
	{
		string group;
		string subfolder;
		string image;
	};

	class MK_OG_EXPORT GorillaAtlas
	{
	public:
		GorillaAtlas(Gorilla::Silverback& silverback, string name, size_t width, size_t height, string path);

		Gorilla::TextureAtlas& atlas() { return *mAtlas; }
		size_t width() { return mWidth; }
		size_t height() { return mHeight; }

		void createAtlas();
		void generateAtlas();
		void saveAtlas(string gorillafontgroup, string gorillafontpath);
		void loadAtlas();

		void fitImage(ImageRect& image);
		void blitImage(Ogre::TexturePtr texture, ImageRect& image);

		void appendSprite(const string& image, const string& group);
		void defineSprite(const string& image, float left, float top, float width, float height);

		std::vector<ImageRect>& fonts() { return mFonts; }
		std::vector<ImageRect>& sprites() { return mSprites; }

	protected:
		Gorilla::Silverback& mSilverback;
		Gorilla::TextureAtlas* mAtlas;
		string mName;
		size_t mWidth;
		size_t mHeight;
		string mPath;
		GuillotineBinPack mRectPacker;
		std::vector<ImageRect> mFonts;
		std::vector<ImageRect> mSprites;

		Ogre::TexturePtr mTexture;
	};

	class MK_OG_EXPORT GorillaLayer : public InkLayer
	{
	public:
		GorillaLayer(GorillaTarget& target, Gorilla::Layer& layer, size_t index);
		~GorillaLayer();

		Gorilla::Layer& layer() { return mLayer; }

		void moved(size_t index);
		unique_ptr<Inkbox> createInkbox(Frame& frame);

		void show();
		void hide();

	protected:
		GorillaTarget& mTarget;
		Gorilla::Layer& mLayer;
	};

	class MK_OG_EXPORT GorillaTarget : public InkTarget
	{
	public:
		GorillaTarget(Gorilla::LayerContainer& screen);

		Gorilla::LayerContainer& screen() { return mScreen; }
		size_t zmax() { return mZMax; }

		unique_ptr<InkLayer> createLayer(Frame& frame, size_t z);

	protected:
		Gorilla::LayerContainer& mScreen;
		size_t mZMax;
	};

	class MK_OG_EXPORT GorillaSpaceTarget : public GorillaTarget
	{
	public:
		GorillaSpaceTarget(Gorilla::ScreenRenderable& spaceScreen);
		~GorillaSpaceTarget();

		Gorilla::ScreenRenderable& spaceScreen() { return mSpaceScreen; }
		Gorilla::Layer& layer() { return mLayer->layer(); }

		void redraw();

	protected:
		Gorilla::ScreenRenderable& mSpaceScreen;
		unique_ptr<GorillaLayer> mLayer;
	};

	class MK_OG_EXPORT GorillaWindow : public InkWindow
	{
	public:
		GorillaWindow(OgreWindow& ogreWindow);
		~GorillaWindow();

		GorillaTarget& gorillaScreenTarget() { return *mScreenTarget.get(); }
		GorillaAtlas& gorillaAtlas() { return *mAtlas.get(); }

		void nextFrame();

		InkTarget& screenTarget();
		InkTarget& spaceTarget(Ogre::SceneManager* sceneMgr, int width, int height);
		void releaseTarget(InkTarget& target);

		const std::vector<unique_ptr<GorillaSpaceTarget>>& spaceTargets() { return mSpaceTargets; }

	protected:
		Ogre::SceneManager& mSceneManager;
		Ogre::Camera& mCamera;
		size_t mAtlasWidth;
		size_t mAtlasHeight;

		unique_ptr<Gorilla::Silverback> mSilverback;
		unique_ptr<GorillaTarget> mScreenTarget;
		std::vector<unique_ptr<GorillaSpaceTarget>> mSpaceTargets;

		unique_ptr<GorillaAtlas> mAtlas;
	};

}

#endif
