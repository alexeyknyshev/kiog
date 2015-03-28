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

#include <Ui/Scheme/mkWDropdown.h>

#include <iostream>

namespace mk
{
	GorillaImage::GorillaImage(GorillaInk* inkbox)
		: mInkbox(inkbox)
		, mFrame(inkbox->frame())
		, mRects(9)
	{
		this->show();
	}
	
	GorillaImage::~GorillaImage()
	{}

	void GorillaImage::prepare()
	{
		ImageSkin& skin = mFrame->inkstyle()->mImageSkin;

		GorillaAtlas* gorillaAtlas = mInkbox->frame()->widget()->uiWindow()->inkWindow()->as<GorillaWindow>()->gorillaAtlas();
		Gorilla::Sprite* sprite = mInkbox->layer()->layer()->_getSprite(skin.d_image);

		float left = sprite->uvLeft * gorillaAtlas->width();
		float top = sprite->uvTop * gorillaAtlas->height();

		for(size_t i = 0; i < 9; ++i)
			gorillaAtlas->defineSprite(skin.d_images[i], left + skin.d_coords[i].x(), top + skin.d_coords[i].y(), skin.d_coords[i].z(), skin.d_coords[i].w());

		skin.d_prepared = true;
	}

	void GorillaImage::show()
	{
		ImageSkin& skin = mFrame->inkstyle()->mImageSkin;

		if(!skin.d_prepared)
			this->prepare();

		for(size_t i = 0; i < 9; ++i)
			mRects[i] = createImage(skin.d_images[i]);
	}

	void GorillaImage::hide()
	{
		for(size_t i = 0; i < 9; ++i)
		{	
			mInkbox->layer()->layer()->destroyRectangle(mRects[i]);
			mRects[i] = nullptr;
		}
	}

	Gorilla::Rectangle* GorillaImage::createImage(const string& image)
	{
		GorillaLayer* layer = mInkbox->layer();
		Gorilla::Sprite* sprite = layer->layer()->_getSprite(image);
		float width = sprite->spriteWidth;
		float height = sprite->spriteHeight;

		Gorilla::Rectangle* rect = mInkbox->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), width, height);
		rect->background_image(image);

		return rect;
	}

	void GorillaImage::updateImage()
	{
		ImageSkin& skin = mFrame->inkstyle()->mImageSkin;

		if(!skin.d_prepared)
			this->prepare();

		for(size_t i = 0; i < 9; ++i)
			mRects[i]->background_image(skin.d_images[i]);
	}

	void GorillaImage::updateFrame()
	{
		FrameSkin fskin(mFrame, &mFrame->inkstyle()->mImageSkin);

		// Borders
		mRects[ImageSkin::TOP]->position(fskin.d_inleft, fskin.d_top);
		mRects[ImageSkin::TOP]->width(fskin.d_inwidth);

		mRects[ImageSkin::RIGHT]->position(fskin.d_inright, fskin.d_intop);
		mRects[ImageSkin::RIGHT]->height(fskin.d_inheight);

		mRects[ImageSkin::BOTTOM]->position(fskin.d_inleft, fskin.d_inbottom);
		mRects[ImageSkin::BOTTOM]->width(fskin.d_inwidth);

		mRects[ImageSkin::LEFT]->position(fskin.d_left, fskin.d_intop);
		mRects[ImageSkin::LEFT]->height(fskin.d_inheight);

		// Corners
		mRects[ImageSkin::TOP_LEFT]->position(fskin.d_outleft, fskin.d_outtop);
		mRects[ImageSkin::TOP_RIGHT]->position(fskin.d_inright, fskin.d_outtop);
		mRects[ImageSkin::BOTTOM_RIGHT]->position(fskin.d_inright, fskin.d_inbottom);
		mRects[ImageSkin::BOTTOM_LEFT]->position(fskin.d_outleft, fskin.d_inbottom);

		// Fill
		mRects[ImageSkin::FILL]->position(fskin.d_inleft, fskin.d_intop);
		mRects[ImageSkin::FILL]->width(fskin.d_inwidth);
		mRects[ImageSkin::FILL]->height(fskin.d_inheight);
	}

	Ogre::ColourValue ogreColour(const Colour& colour)
	{
		return Ogre::ColourValue(colour.r(), colour.g(), colour.b(), colour.a());
	}

	GorillaInk::GorillaInk(Frame* frame, GorillaLayer* layer)
		: Inkbox(frame)
		, mLayer(layer)
		, mRect(nullptr)
		, mImage(nullptr)
		, mCaption(nullptr)
		, mImageSkin(nullptr)
		, mLastRect(nullptr)
	{}

	GorillaInk::~GorillaInk()
	{
		if(mVisible)
			this->hide();
	}

	Gorilla::Rectangle* GorillaInk::lastRect()
	{
		if(mLastRect)
			return mLastRect;
		else if(mFrame->frameType() == LAYER)
			return nullptr;
		
		Frame* frame = mFrame;
		while(frame->index() > 0)
		{
			frame = frame->prev();
			if(frame->frameType() < LAYER)
				return static_cast<GorillaInk*>(frame->inkbox())->lastRect();
		}

		return static_cast<GorillaInk*>(mFrame->parent()->inkbox())->lastRect();
	}

	Gorilla::Rectangle* GorillaInk::createRectangle(Ogre::Real left, Ogre::Real top, Ogre::Real width, Ogre::Real height)
	{
		size_t index = 0;
		Gorilla::Rectangle* lastRect = this->lastRect();
		if(lastRect)
			index = lastRect->index() + 1;

		mLastRect = mLayer->layer()->insertRectangle(index, left, top, width, height);
		return mLastRect;
	}

	void GorillaInk::show()
	{
		if(!mFrame->inkstyle()->mEmpty)
			mRect = this->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y));

		mVisible = true;

		if(mImageSkin)
			mImageSkin->show();

		this->updateStyle();
		this->updateContent();
	}

	void GorillaInk::hide()
	{
		if(mRect)
		{
			mLayer->layer()->destroyRectangle(mRect);
			mRect = nullptr;
		}
		if(mImage)
		{
			mLayer->layer()->destroyRectangle(mImage);
			mImage = nullptr;
		}
		if(mCaption)
		{
			mLayer->layer()->destroyCaption(mCaption);
			mCaption = nullptr;
		}
		if(mImageSkin)
		{
			mImageSkin->hide();
		}

		mLastRect = nullptr;
		mVisible = false;
	}

	float GorillaInk::contentSize(Dimension dim)
	{
		if(mImage)
		{
			Gorilla::Sprite* sprite = mLayer->layer()->_getSprite(mFrame->widget()->image());
			float xoffset = mFrame->inkstyle()->mPadding[DIM_X] * 2.f;
			float yoffset = mFrame->inkstyle()->mPadding[DIM_Y] * 2.f;

			return dim == DIM_X ? sprite->spriteWidth + xoffset: sprite->spriteHeight + yoffset;
		}
		if(mCaption)
		{
			Ogre::Vector2 size;
			mCaption->_calculateDrawSize(size);
			float xoffset = mFrame->inkstyle()->mPadding[DIM_X] * 2.f;
			float yoffset = mFrame->inkstyle()->mPadding[DIM_Y] * 2.f;
			xoffset += 3.f; // hardcoded tweak because gorilla reports a slightly smaller size than it needs

			return dim == DIM_X ? size.x + xoffset : size.y + yoffset;
		}
		
		return 0.f;
	}

	void GorillaInk::updateContent()
	{
		this->updateImage();
		this->updateCaption();
	}

	void GorillaInk::updateStyle()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mFrame->inkstyle()->mBackgroundColour.a() == 0.f)
			mRect->no_background();
		else
			mRect->background_colour(ogreColour(mFrame->inkstyle()->mBackgroundColour));

		if(mFrame->inkstyle()->mBorderWidth.x())
			mRect->border(mFrame->inkstyle()->mBorderWidth.x(), ogreColour(mFrame->inkstyle()->mBorderColour));

		if(!mFrame->inkstyle()->mImageSkin.null() && !mImageSkin)
			mImageSkin = make_unique<GorillaImage>(this);
		else if(!mFrame->inkstyle()->mImageSkin.null() && mImageSkin)
			mImageSkin->updateImage();

		// @todo add gradients
	}

	void GorillaInk::updateFrame()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible || mFrame->dsize(DIM_X) == 0.f || mFrame->dsize(DIM_X) == 0.f)
			return;
		
		float left = mFrame->dabsolute(DIM_X) + mFrame->dclippos(DIM_X) + mFrame->inkstyle()->mMargin[DIM_X] + mFrame->inkstyle()->mBorderWidth.x();
		float top = mFrame->dabsolute(DIM_Y) + mFrame->dclippos(DIM_Y) + mFrame->inkstyle()->mMargin[DIM_Y] + mFrame->inkstyle()->mBorderWidth.x();

		float width = mFrame->dclipsize(DIM_X) - mFrame->inkstyle()->mMargin[DIM_X] - mFrame->inkstyle()->mMargin[DIM_X + 2] - mFrame->inkstyle()->mBorderWidth[DIM_X] - mFrame->inkstyle()->mBorderWidth[DIM_X + 2];
		float height = mFrame->dclipsize(DIM_Y) - mFrame->inkstyle()->mMargin[DIM_Y] - mFrame->inkstyle()->mMargin[DIM_Y + 2] - mFrame->inkstyle()->mBorderWidth[DIM_Y] - mFrame->inkstyle()->mBorderWidth[DIM_Y + 2];

		mRect->left(left);
		mRect->top(top);

		mRect->width(width);
		mRect->height(height);

		if(mCaption)
		{
			mCaption->left(left + mFrame->inkstyle()->mPadding[DIM_X]);
			mCaption->top(top + mFrame->inkstyle()->mPadding[DIM_Y]);

			mCaption->size(width - mFrame->inkstyle()->mPadding[DIM_X] * 2.f, height - mFrame->inkstyle()->mPadding[DIM_Y] * 2.f);
		}

		if(mImage)
		{
			mImage->left(left + mFrame->inkstyle()->mPadding[DIM_X]);
			mImage->top(top + mFrame->inkstyle()->mPadding[DIM_Y]);
			//mImage->width(width - mFrame->inkstyle()->mPadding[DIM_X] * 2.f);
			//mImage->height(height - mFrame->inkstyle()->mPadding[DIM_Y] * 2.f);
		}

		if(mImageSkin)
			mImageSkin->updateFrame();

		//if(mFrame->style()->name() != "cursor")
		//	std::cerr << "GorillaInk :: draw " << mFrame->style()->name() << " at " << mRect->left() << " , " << mRect->top() << " size " << mRect->width() << " , " << mRect->height() << std::endl;
	}

	void GorillaInk::updateImage()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mImage && mFrame->widget()->image().empty())
		{
			mLayer->layer()->destroyRectangle(mImage);
			mImage = nullptr;
			return;
		}
		else if(!mImage && !mFrame->widget()->image().empty())
		{
			Gorilla::Sprite* sprite = mLayer->layer()->_getSprite(mFrame->widget()->image());
			mImage = this->createRectangle(mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), sprite->spriteWidth, sprite->spriteHeight);
		}

		if(!mFrame->widget()->image().empty())
		{
			mImage->background_image(Ogre::String(mFrame->widget()->image()));
		}
	}

	void GorillaInk::updateCaption()
	{
		if(mFrame->inkstyle()->mEmpty || !mVisible)
			return;

		if(mCaption && mFrame->widget()->label().empty())
		{
			mLayer->layer()->destroyCaption(mCaption);
			mCaption = nullptr;
			return;
		}
		else if(!mCaption && !mFrame->widget()->label().empty())
		{
			mCaption = mLayer->layer()->createCaption(9, mFrame->dabsolute(DIM_X), mFrame->dabsolute(DIM_Y), mFrame->widget()->label());
		}

		if(!mFrame->widget()->label().empty())
		{
			mCaption->text(mFrame->widget()->label());
			mCaption->size(mFrame->dsize(DIM_X), mFrame->dsize(DIM_Y));
			mCaption->align(Gorilla::TextAlign_Left);
			mCaption->vertical_align(Gorilla::VerticalAlign_Middle);
			mCaption->no_background();
			mCaption->colour(ogreColour(mFrame->inkstyle()->mTextColour));
		}
	}
}
