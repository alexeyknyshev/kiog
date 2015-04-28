//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Gorilla/mkGorillaWindow.h>

#include <Og/Ogre/mkOgreModule.h>

#include <Ui/Frame/mkFrame.h>
#include <Og/Gorilla/mkGorillaInk.h>

#include <Compositor/OgreCompositorManager2.h>

#include <Object/Util/mkMake.h>

#include <Og/Gorilla/RectPacking/GuillotineBinPack.h>
#include <Og/Gorilla/RectPacking/SkylineBinPack.h>

#include <iostream>
#include <fstream>

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR > 0
namespace Ogre
{
	typedef v1::HardwarePixelBufferSharedPtr HardwarePixelBufferSharedPtr;
	typedef v1::HardwareBuffer HardwareBuffer;
}
#endif

namespace mk
{
	ImageRect makeRect(const string& image, const string& group, const string& subfolder = "")
	{
		ImageRect ret;
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->load(image, group);
		ret.image = image;
		ret.group = group;
		ret.subfolder = subfolder;
		ret.width = tex->getWidth();
		ret.height = tex->getHeight();
		Ogre::TextureManager::getSingletonPtr()->remove(tex->getHandle());
		Ogre::TextureManager::getSingletonPtr()->unload(image);
		return ret;
	}

	unique_ptr<GorillaAtlas> generateAtlas(Gorilla::Silverback& silverback, size_t atlasWidth, size_t atlasHeight, string resourcePath)
	{
		string imagefontgroup = "imagefonts";
		string gorillafontgroup = "gorillafonts";
		string spritegroup = "uisprites";

		string gorillafontpath = resourcePath + "interface/gorillafonts/";
		string outputpath = resourcePath + "interface/gorilla/";

		unique_ptr<GorillaAtlas> atlas = make_unique<GorillaAtlas>(silverback, "uiatlas", atlasWidth, atlasHeight, outputpath);

		Ogre::StringVectorPtr fonts = Ogre::ResourceGroupManager::getSingletonPtr()->listResourceNames(imagefontgroup);
		Ogre::StringVectorPtr sprites = Ogre::ResourceGroupManager::getSingletonPtr()->listResourceNames(spritegroup);
		Ogre::StringVectorPtr spritesubs = Ogre::ResourceGroupManager::getSingletonPtr()->listResourceNames(spritegroup, true);

		for(string& font : *fonts)
			atlas->fonts().push_back(makeRect(font, imagefontgroup));

		for(string& sprite : *sprites)
			atlas->sprites().push_back(makeRect(sprite, spritegroup));

		for(string& spritefolder : *spritesubs)
		{
			Ogre::StringVectorPtr subsprites = Ogre::ResourceGroupManager::getSingletonPtr()->listResourceNames(spritefolder);
			for(string& sprite : *subsprites)
				atlas->sprites().push_back(makeRect(sprite, spritefolder, spritefolder + "/"));
		}

		atlas->createAtlas();
		atlas->generateAtlas();
		atlas->saveAtlas(gorillafontgroup, gorillafontpath);
		atlas->loadAtlas();

		return atlas;
	}

	GorillaAtlas::GorillaAtlas(Gorilla::Silverback& silverback, string name, size_t width, size_t height, string path)
		: mSilverback(silverback)
		, mName(name)
		, mWidth(width)
		, mHeight(height)
		, mPath(path)
		, mRectPacker(width, height)
	{}

	void GorillaAtlas::createAtlas()
	{
		// Create the atlas texture

		mTexture = Ogre::TextureManager::getSingletonPtr()->createManual(mName + ".png",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			mWidth,
			mHeight,
			0,
			Ogre::PF_A8R8G8B8,
			Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE
			);

		// Write white pixel (4 * 4) at width - 4 and height - 4

		Ogre::Image::Box destBox(mWidth - 4, mHeight - 4, 0, mWidth, mHeight, 1);
		const Ogre::PixelBox& destPixelBox = mTexture->getBuffer()->lock(destBox, Ogre::HardwareBuffer::HBL_NORMAL);

		Ogre::uint8* data = static_cast<Ogre::uint8*>(destPixelBox.data);

		for(size_t y = 0; y < 4; ++y)
		{
			for(size_t x = 0; x < 4; ++x, data += 4)
				Ogre::PixelUtil::packColour((Ogre::uint8)255, (Ogre::uint8)255, (Ogre::uint8)255, (Ogre::uint8)255, destPixelBox.format, data);

			data += destPixelBox.getRowSkip() * 4;
		}

		mTexture->getBuffer()->unlock();
	}

	void GorillaAtlas::generateAtlas()
	{
		// @todo : sort images

		// Fit the sprites where we can
		for(ImageRect& image : mFonts)
			this->fitImage(image);

		for(ImageRect& image : mSprites)
			this->fitImage(image);

		// Blit the sprites to texture
		for(ImageRect& image : mFonts)
			this->blitImage(mTexture, image);

		for(ImageRect& image : mSprites)
			this->blitImage(mTexture, image);
	}

	void GorillaAtlas::saveAtlas(string gorillafontgroup, string gorillafontpath)
	{
		// Save the atlas to disk

		mTexture->getBuffer()->lock(Ogre::HardwareBuffer::HBL_NORMAL);

		Ogre::Image img;
		img.loadDynamicImage(static_cast<Ogre::uchar*>(mTexture->getBuffer()->getCurrentLock().data), mWidth, mHeight, mTexture->getDepth(), mTexture->getFormat());
		img.save(mPath + mName + ".png");

		mTexture->getBuffer()->unlock();

		// Now write the gorilla file
		std::ofstream file;
		file.open(mPath + mName + ".gorilla");

		file << "[Texture]" << std::endl;
		file << "file " << mName << ".png" << std::endl;
		file << "whitepixel " << mWidth - 2 << " " << mHeight - 2 << std::endl;
		file << std::endl;

		size_t i = 0;
		Ogre::StringVectorPtr gorillafonts = Ogre::ResourceGroupManager::getSingletonPtr()->listResourceNames(gorillafontgroup);
		for(ImageRect& font : mFonts)
		{
			string fontName = gorillafonts->at(i++);
			file << "[" << fontName << "]" << std::endl;;
			file << "offset " << font.x << " " << font.y << std::endl;

			std::ifstream fontfile;

			// Ogre::String gorillafontpath = Ogre::ResourceGroupManager::getSingletonPtr()->getResourceLocationList(gorillafontgroup).front();
			// Didn't find a way to get this path

			fontfile.open(gorillafontpath + fontName);
			file << fontfile.rdbuf();
			file << std::endl;
			file << std::endl;
		}

		file << "[Sprites]" << std::endl;
		for(ImageRect& sprite : mSprites)
				file << sprite.subfolder << sprite.image << " " << sprite.x << " " << sprite.y << " " << sprite.width << " " << sprite.height << std::endl;

		file.close();
	}

	void GorillaAtlas::loadAtlas()
	{
		mAtlas = mSilverback.loadAtlas(mName, "gorilla");
	}

	void GorillaAtlas::fitImage(ImageRect& image)
	{
		static_cast<BPRect&>(image) = mRectPacker.Insert(image.width, image.height, false, GuillotineBinPack::RectBestShortSideFit, GuillotineBinPack::SplitShorterLeftoverAxis);
	}

	void GorillaAtlas::blitImage(Ogre::TexturePtr texture, ImageRect& image)
	{
		Ogre::Image::Box destBox(image.x, image.y, 0, image.x + image.width, image.y + image.height, 1);

		const Ogre::PixelBox& destPixelBox = texture->getBuffer()->lock(destBox, Ogre::HardwareBuffer::HBL_WRITE_ONLY);

		Ogre::TexturePtr tex = Ogre::TextureManager::getSingletonPtr()->load(image.image, image.group);
		Ogre::HardwarePixelBufferSharedPtr sourceBuffer = Ogre::TextureManager::getSingletonPtr()->getByName(image.image, image.group)->getBuffer();
		sourceBuffer->blitToMemory(destPixelBox);
		Ogre::TextureManager::getSingletonPtr()->remove(tex->getHandle());
		Ogre::TextureManager::getSingletonPtr()->unload(image.image);
		// @note these fucking ugly kludges are here to workaround Ogre being unable to have two textures with the same name in two different resource groups at the same time

		texture->getBuffer()->unlock();
	}

	void GorillaAtlas::appendSprite(const string& name, const string& group)
	{
		mSprites.push_back(makeRect(name, group));

		ImageRect& sprite = mSprites.back();

		this->fitImage(sprite);

		this->blitImage(mAtlas->getTexture(), sprite);
		mAtlas->addSprite(name, sprite.x, sprite.y, sprite.width, sprite.height);
	}

	void GorillaAtlas::defineSprite(const string& name, float left, float top, float width, float height)
	{
		mAtlas->addSprite(name, left, top, width, height);
	}

	GorillaLayer::GorillaLayer(Layer& layer, GorillaTarget& target, size_t index)
		: InkLayer(layer, target, index)
		, mTarget(target)
		, mLayer(*target.screen().createLayer(index))
	{}

	GorillaLayer::~GorillaLayer()
	{
		mTarget.screen().destroy(&mLayer);
	}

	void GorillaLayer::moved(size_t index)
	{
		mTarget.screen().moveLayer(&mLayer, index);
	}

	unique_ptr<Inkbox> GorillaLayer::createInkbox(Frame& frame)
	{
		return make_unique<GorillaInk>(frame, *this);
	}

	void GorillaLayer::show()
	{
		mLayer.show();
	}

	void GorillaLayer::hide()
	{
		mLayer.hide();
	}

	GorillaTarget::GorillaTarget(Gorilla::LayerContainer& screen)
		: InkTarget(15)
		, mScreen(screen)
	{}

	unique_ptr<InkLayer> GorillaTarget::createLayer(Layer& layer, size_t z)
	{
		return make_unique<GorillaLayer>(layer, *this, z);
	}

	GorillaSpaceTarget::GorillaSpaceTarget(Gorilla::ScreenRenderable& spaceScreen)
		: GorillaTarget(spaceScreen)
		, mSpaceScreen(spaceScreen)
	{}
	
	GorillaSpaceTarget::~GorillaSpaceTarget()
	{}

	void GorillaSpaceTarget::redraw()
	{
		mSpaceScreen._requestIndexRedraw(0);
	}

	GorillaWindow::GorillaWindow(OgreWindow& ogreWindow)
		: mSceneManager(*ogreWindow.ogreModule().ogreRoot().createSceneManager(Ogre::ST_GENERIC, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD))
		, mCamera(*mSceneManager.createCamera("GorillaUi"))
		, mSilverback(make_unique<Gorilla::Silverback>())
		, mAtlasWidth(1024)
		, mAtlasHeight(1024)
	{
		mAtlas = generateAtlas(*mSilverback.get(), mAtlasWidth, mAtlasHeight, ogreWindow.ogreModule().resourcePath());

		mScreenTarget = make_unique<GorillaTarget>(*mSilverback->createScreen(&mSceneManager, &ogreWindow.context(), "uiatlas"));
	}

	GorillaWindow::~GorillaWindow()
	{}

	void GorillaWindow::nextFrame()
	{}

	InkTarget& GorillaWindow::screenTarget()
	{
		return *mScreenTarget.get();
	}

	InkTarget& GorillaWindow::spaceTarget(Ogre::SceneManager* sceneMgr, int width, int height)
	{
		mSpaceTargets.emplace_back(make_unique<GorillaSpaceTarget>(*mSilverback->createScreenRenderable(sceneMgr, Ogre::Vector2(width, height), "uiatlas")));
		return *mSpaceTargets.back().get();
	}

	void GorillaWindow::releaseTarget(InkTarget& target)
	{
		size_t pos = 0;
		while(mSpaceTargets[pos].get() != &target)
			++pos;

		mSilverback->destroyScreenRenderable(&mSpaceTargets[pos]->spaceScreen());
		mSpaceTargets.erase(mSpaceTargets.begin() + pos);
	}
}
