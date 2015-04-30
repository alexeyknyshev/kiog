//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Gorilla/mkGorillaInk.h>

#include <Object/Util/mkColour.h>
#include <Ui/Frame/mkFrame.h>
#include <Ui/Frame/mkStripe.h>
#include <Ui/Widget/mkWidget.h>

#include <Ui/mkUiWindow.h>
#include <Og/Gorilla/mkGorillaWindow.h>

#include <OgreColourValue.h>

#include <iostream>

#include <Ui/Widget/mkSheet.h>

namespace mk
{
	GorillaImage::GorillaImage(GorillaInk& inkbox)
		: mInkbox(inkbox)
		, mFrame(inkbox.frame())
		, mRects(9)
	{
		//this->show();
	}
	
	GorillaImage::~GorillaImage()
	{
		if(mRects[0])
			this->hide();
	}

	void GorillaImage::prepare()
	{
		ImageSkin& skin = mFrame.inkstyle().imageSkin();

		if(skin.d_prepared)
			return;

		GorillaAtlas& gorillaAtlas = mInkbox.frame().widget().uiWindow().inkWindow().as<GorillaWindow>().gorillaAtlas();
		Gorilla::Sprite& sprite = *mInkbox.layer().layer()._getSprite(skin.d_image.d_name + ".png");

		float left = sprite.uvLeft * gorillaAtlas.width();
		float top = sprite.uvTop * gorillaAtlas.height();

		skin.prepare(sprite.spriteWidth, sprite.spriteHeight);

		for(size_t i = 0; i < 9; ++i)
			gorillaAtlas.defineSprite(skin.d_images[i] + ".png", left + skin.d_coords[i].x0(), top + skin.d_coords[i].y0(), skin.d_coords[i].x1(), skin.d_coords[i].y1());

		skin.d_prepared = true;
	}

	void GorillaImage::show()
	{
		const ImageSkin& skin = mFrame.inkstyle().mImageSkin;

		this->prepare();

		for(size_t i = 0; i < 9; ++i)
			mRects[i] = createImage(skin.d_images[i]);
	}

	void GorillaImage::hide()
	{
		for(size_t i = 0; i < 9; ++i)
		{	
			mInkbox.layer().layer().destroyRectangle(mRects[i]);
			mRects[i] = nullptr;
		}
	}

	Gorilla::Rectangle* GorillaImage::createImage(const string& image)
	{
		GorillaLayer& layer = mInkbox.layer();
		Gorilla::Sprite& sprite = *layer.layer()._getSprite(image + ".png");
		float width = sprite.spriteWidth;
		float height = sprite.spriteHeight;

		Gorilla::Rectangle* rect = mInkbox.createRectangle(mFrame.dabsolute(DIM_X), mFrame.dabsolute(DIM_Y), width, height);
		rect->background_image(image + ".png");

		return rect;
	}

	void GorillaImage::updateImage()
	{
		const ImageSkin& skin = mFrame.inkstyle().mImageSkin;

		this->prepare();

		for(size_t i = 0; i < 9; ++i)
			mRects[i]->background_image(skin.d_images[i] + ".png");
	}

	void GorillaImage::updateFrame()
	{
		float width = mFrame.cwidth();
		float height = mFrame.cheight();
		float left = mFrame.dabsolute(DIM_X);
		float top = mFrame.dabsolute(DIM_Y);

		const ImageSkin& imgskin = mFrame.inkstyle().mImageSkin;
		float margin = imgskin.d_margin * 2.f;
		imgskin.stretchCoords(width + margin, height + margin, [this, left, top, margin](ImageSkin::Section s, int x, int y, int w, int h)
		{
			this->mRects[s]->position(left + x - margin * 0.5f, top + y - margin * 0.5f);
			this->mRects[s]->width(w);
			this->mRects[s]->height(h);
		});
	}

	Ogre::ColourValue ogreColour(const Colour& colour)
	{
		return Ogre::ColourValue(colour.r(), colour.g(), colour.b(), colour.a());
	}

	GorillaInk::GorillaInk(Frame& frame, GorillaLayer& layer)
		: Inkbox(frame)
		, mLayer(layer)
		, mRect(nullptr)
		, mImage(nullptr)
		, mCaption(nullptr)
		, mImageSkin(nullptr)
	{}

	GorillaInk::~GorillaInk()
	{
		if(mVisible)
			this->hide();
	}

	Gorilla::Rectangle* GorillaInk::lastRect()
	{
		if(mImageSkin && mImageSkin->lastRect())
			return mImageSkin->lastRect();
		else if(mImage)
			return mImage;
		else if(mRect)
			return mRect;
		else if(mFrame.frameType() == LAYER)
			return nullptr;
		
		Frame* frame = &mFrame;
		while(frame->index() > 0)
		{
			frame = frame->prev();
			if(frame->frameType() < LAYER)
				return static_cast<GorillaInk&>(frame->inkbox()).lastRect();
		}

		return static_cast<GorillaInk&>(mFrame.parent()->inkbox()).lastRect();
	}

	Gorilla::Rectangle* GorillaInk::createRectangle(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height)
	{
		size_t index = 0;
		Gorilla::Rectangle* lastRect = this->lastRect();
		if(lastRect)
			index = lastRect->index() + 1;

		return mLayer.layer().insertRectangle(index, left, top, width, height);
	}

	void GorillaInk::show()
	{
		if(!skin().mEmpty)
			mRect = this->createRectangle(mFrame.dabsolute(DIM_X), mFrame.dabsolute(DIM_Y), mFrame.dsize(DIM_X), mFrame.dsize(DIM_Y));

		mVisible = true;

		if(mImageSkin && !skin().imageSkin().null())
			mImageSkin->show();

		this->updateContent();
		this->updateStyle();
	}

	void GorillaInk::hide()
	{
		if(!mVisible)
			return;

		if(mRect)
		{
			mLayer.layer().destroyRectangle(mRect);
			mRect = nullptr;
		}
		if(mImage)
		{
			mLayer.layer().destroyRectangle(mImage);
			mImage = nullptr;
		}
		if(mCaption)
		{
			mLayer.layer().destroyCaption(mCaption);
			mCaption = nullptr;
		}
		if(mImageSkin)
		{
			mImageSkin->hide();
		}

		mVisible = false;
	}

	float GorillaInk::contentSize(Dimension dim)
	{
		if(mImage)
		{
			Gorilla::Sprite* sprite = mLayer.layer()._getSprite(mFrame.widget().image()->d_name + ".png");
			float xoffset = skin().padding()[DIM_X] + skin().padding()[DIM_X + 2];
			float yoffset = skin().padding()[DIM_Y] + skin().padding()[DIM_Y + 2];

			return dim == DIM_X ? sprite->spriteWidth + xoffset: sprite->spriteHeight + yoffset;
		}
		if(mCaption)
		{
			Ogre::Vector2 size;
			mCaption->_calculateDrawSize(size);
			float xoffset = skin().padding()[DIM_X] + skin().padding()[DIM_X + 2];
			float yoffset = skin().padding()[DIM_Y] + skin().padding()[DIM_Y + 2];
			xoffset += 3.f; // hardcoded tweak because gorilla reports a slightly smaller size than it needs

			return dim == DIM_X ? size.x + xoffset : size.y + yoffset;
		}
		
		return 0.f;
	}

	size_t GorillaInk::caretIndex(float x, float y)
	{
		if(!mCaption)
			return 0;
		return mCaption->_calculateCaretIndex(x);
	}

	void GorillaInk::caretCoords(size_t index, float& caretX, float& caretY, float& caretHeight)
	{
		if(mCaption)
		{
			caretY = 0.f;
			mCaption->_calculateCaretCoords(index, caretX, caretHeight);
			caretX += skin().padding()[DIM_X];
			caretY += skin().padding()[DIM_Y];
		}
	}

	void GorillaInk::updateContent()
	{
		this->updateImage();
		this->updateCaption();
	}

	void GorillaInk::updateStyle()
	{
		if(!mVisible) //  || skin().mEmpty
			return;

		if(mRect)
		{
			if(skin().backgroundColour().a() == 0.f)
				mRect->no_background();
			else
				mRect->background_colour(ogreColour(skin().mBackgroundColour));

			if(skin().borderWidth().x0())
				mRect->border(skin().borderWidth().x0(), ogreColour(skin().mBorderColour));
			else
				mRect->no_border();
		}

		if(!skin().imageSkin().null() && !mImageSkin)
		{
			mImageSkin = make_unique<GorillaImage>(*this);
			mImageSkin->show();
		}
		else if(!skin().imageSkin().null() && mImageSkin)
			mImageSkin->updateImage();
		else if(skin().imageSkin().null() && mImageSkin)
			mImageSkin.reset();

		this->updateImage();

		// @todo add gradients
	}

	void GorillaInk::updateClip()
	{
		this->updateFrame();
	}

	void GorillaInk::updatePosition()
	{
		this->updateFrame();
	}

	void GorillaInk::updateFrame()
	{
		if(skin().mEmpty || !mVisible || mFrame.dsize(DIM_X) == 0.f || mFrame.dsize(DIM_Y) == 0.f)
			return;
		
		float left = mFrame.dabsolute(DIM_X) + mFrame.dclippos(DIM_X) + skin().margin().x0() + skin().borderWidth().x0();
		float top = mFrame.dabsolute(DIM_Y) + mFrame.dclippos(DIM_Y) + skin().margin().y0() + skin().borderWidth().x0();

		float width = mFrame.dclipsize(DIM_X) - skin().margin().x0() - skin().margin().x1() - skin().borderWidth().x0() - skin().borderWidth().x1();
		float height = mFrame.dclipsize(DIM_Y) - skin().margin().y0() - skin().margin().y1() - skin().borderWidth().y0() - skin().borderWidth().y1();

		if(mRect)
		{
			mRect->left(left);
			mRect->top(top);

			mRect->width(width);
			mRect->height(height);
		}

		if(mCaption)
		{
			mCaption->left(left + skin().padding()[DIM_X]);
			mCaption->top(top + skin().padding()[DIM_Y]);

			mCaption->size(width - skin().padding()[DIM_X] * 2.f, height - skin().padding()[DIM_Y] * 2.f);
		}

		if(mImage)
		{
			mImage->left(left + skin().padding()[DIM_X]);
			mImage->top(top + skin().padding()[DIM_Y]);
			//mImage->width(width - skin().padding()[DIM_X] * 2.f);
			//mImage->height(height - skin().padding()[DIM_Y] * 2.f);
		}

		if(mImageSkin)
			mImageSkin->updateFrame();

		//if(mFrame.style()->name() != "cursor")
		//	std::cerr << "GorillaInk :: draw " << mFrame.style()->name() << " at " << mRect->left() << " , " << mRect->top() << " size " << mRect->width() << " , " << mRect->height() << std::endl;
	}

	void GorillaInk::updateImage()
	{
		if(skin().mEmpty || !mVisible)
			return;

		if(mImage && !mFrame.widget().image())
		{
			mLayer.layer().destroyRectangle(mImage);
			mImage = nullptr;
			return;
		}
		else if(!mImage && mFrame.widget().image())
		{
			Gorilla::Sprite* sprite = mLayer.layer()._getSprite(mFrame.widget().image()->d_name + ".png");
			mImage = this->createRectangle(mFrame.dabsolute(DIM_X), mFrame.dabsolute(DIM_Y), sprite->spriteWidth, sprite->spriteHeight);
		}
		if(mFrame.widget().image())
		{
			Gorilla::Sprite* sprite = mLayer.layer()._getSprite(mFrame.widget().image()->d_name + ".png");
			mImage->background_image(Ogre::String(mFrame.widget().image()->d_name + ".png"));
			mImage->width(sprite->spriteWidth);
			mImage->height(sprite->spriteHeight);
		}
	}

	void GorillaInk::updateCaption()
	{
		if(skin().mEmpty || !mVisible)
			return;

		if(mCaption && skin().textColour().a() == 0.f) // mFrame.widget().label().empty())
		{
			mLayer.layer().destroyCaption(mCaption);
			mCaption = nullptr;
			return;
		}
		else if(!mCaption && skin().textColour().a() != 0.f) // !mFrame.widget().label().empty())
		{
			mCaption = mLayer.layer().createCaption(9, mFrame.dabsolute(DIM_X), mFrame.dabsolute(DIM_Y), mFrame.widget().label());
		}

		if(skin().textColour().a() != 0.f) // !mFrame.widget().label().empty())
		{
			mCaption->text(mFrame.widget().label());
			mCaption->size(mFrame.dsize(DIM_X), mFrame.dsize(DIM_Y));
			mCaption->align(Gorilla::TextAlign_Left);
			mCaption->vertical_align(Gorilla::VerticalAlign_Top);
			mCaption->no_background();
			mCaption->colour(ogreColour(skin().mTextColour));
		}
	}
}
