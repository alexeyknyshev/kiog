//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_WOGREVIEWPORT_H_INCLUDED
#define MK_WOGREVIEWPORT_H_INCLUDED

/* mk Headers */
#include <Og/mkOgForward.h>
#include <Og/Ogre/mkOgViewport.h>
#include <Ui/Widget/mkSheet.h>

#ifdef GORILLA_V21
#include <Og/Gorilla/Gorilla21.h>
#else
#include <Og/Gorilla/Gorilla.h>
#endif

namespace mk
{
	class MK_OG_EXPORT _I_ WOgViewport : public Sheet, public OgViewport, public Typed<WOgViewport>, public Styled<WOgViewport>
	{
	public:
		WOgViewport(Form* form, Ogre::Camera* camera, MOC::CollisionTools* collisionTools = nullptr);
		~WOgViewport();

		void build();

		void resize();

		void initViewport();
		void initTexture();
		void resizeTexture();

		void nextFrame(size_t tick, size_t delta);
		
		bool leftClick(float x, float y);
		bool rightClick(float x, float y);

		void viewportRay(float x, float y, float* from, float* to);

		virtual void leftPicked(Object* object, bool shift, float x, float y) = 0;
		virtual void rightPicked(Object* object, bool shift, float x, float y) = 0;
		
		using Typed<WOgViewport>::cls;

	protected:
		OgreWindow* mOgreWindow;
		GorillaWindow* mGorillaWindow;

		string mMaterialName;
		Ogre::RenderWindow* mRenderWindow;
		Ogre::Root* mRoot;
		Ogre::TexturePtr mTexture;
		Ogre::RenderTexture* mRenderTexture;
		Ogre::MaterialPtr mMaterial;
		Ogre::CompositorWorkspace* mWorkspace;

		Gorilla::Viewport* mGorillaViewport;

		std::function<void(float, float, btVector3&, btVector3&)> mViewportRay;

		bool mBlank;

		unique_ptr<Controller> mController;
	};
}

#endif // MK_OGRECEGUIVIEWPORT_H_INCLUDED
