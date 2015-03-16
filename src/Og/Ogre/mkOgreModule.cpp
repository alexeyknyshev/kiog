//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/mkOgreModule.h>

#include <Ui/Frame/mkInk.h>
#include <Ui/mkUiWindow.h>

#include <Og/Ogre/mkOgViewport.h>

/* Ogre */
#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreBuildSettings.h>

#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClearDef.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassSceneDef.h>
#include <Compositor/Pass/PassQuad/OgreCompositorPassQuadDef.h>
#include <OgrePass.h>

#include <Compositor/OgreTextureDefinition.h>

namespace mk
{
	OgreWindow::OgreWindow(OgreModule* module, UiWindow* window, Ogre::RenderWindow* context)
		: RenderWindow(context->getWidth(), context->getHeight(), context->getName(), 0)
		, mModule(module)
		, mWindow(window)
		, mContext(context)
	{
		Ogre::WindowEventUtilities::addWindowEventListener(mContext, this);

		mContext->getCustomAttribute("WINDOW", &mHandle);
	}

	OgreWindow::~OgreWindow()
	{
		Ogre::WindowEventUtilities::removeWindowEventListener(mContext, this);
	}

	void OgreWindow::updateSize()
	{
		unsigned int depth;
		int left, top;
		mContext->getMetrics(mWidth, mHeight, depth, left, top);
		mWindow->resize(mWidth, mHeight);
	}

	void OgreWindow::windowResized(Ogre::RenderWindow* renderWindow)
	{
		this->updateSize();
	}

	void OgreWindow::windowFocusChange(Ogre::RenderWindow* renderWindow)
	{
		mActive = !mActive;
		mModule->focusChange();
		//std::cerr << "Window focus change : " << renderWindow->isActive() << " , " << renderWindow->isVisible() << std::endl;
		//std::cerr << "Now set to : " << mActive << std::endl;
	}

	void OgreWindow::windowClosed(Ogre::RenderWindow* renderWindow)
	{
		//Unattach OIS before window shutdown (very important under Linux)
		//destroyInput();
		mShutdown = true;
	}


 	OgreModule::OgreModule(const string& pluginsPath, const string& resourcePath)
		: mOgreRoot(new/*front*/Ogre::Root(pluginsPath, resourcePath + "ogre.cfg", resourcePath + "ogre.log"))
		, mResourcePath(resourcePath)
	{}

	OgreModule::~OgreModule()
	{
		if(mOgreRoot)
			mOgreRoot->shutdown();
		delete mOgreRoot;
	}

	void OgreModule::initStart()
	{
		this->setupResources();
	}

	void OgreModule::configPrompt()
	{
		mOgreRoot->showConfigDialog();
		mOgreRoot->initialise(false);
		//mOgreRoot->initialise();

		this->setupHiddenWindow();
	}

	void OgreModule::configAuto()
	{
#ifdef GORILLA_V21
		this->setupRenderer("OpenGL 3+ Rendering Subsystem");
#else
		this->setupRenderer("OpenGL Rendering Subsystem");
#endif

		// initialize without creating window
		mOgreRoot->getRenderSystem()->setConfigOption("Full Screen", "No");
		mOgreRoot->saveConfig();
		mOgreRoot->initialise(false);

		this->setupHiddenWindow();
	}

	void OgreModule::initEnd()
	{
		Ogre::CompositorManager2* compositorManager = mOgreRoot->getCompositorManager2();
		compositorManager->createBasicWorkspaceDef("Basic Workspace", Ogre::ColourValue(0.15f, 0.15f, 0.15f));
		compositorManager->createBasicWorkspaceDef("Black Workspace", Ogre::ColourValue::Black);

		this->loadResources();
	}

	void OgreModule::multiViewportWorkspace(const std::vector<OgViewport*>& viewports, Ogre::RenderTarget* target)
	{
		Ogre::CompositorManager2* compositorManager = mOgreRoot->getCompositorManager2();
		Ogre::CompositorNodeDef* nodeDef = compositorManager->addNodeDefinition("AutoGen " + (Ogre::IdString("MultiViewportWorkspace") + Ogre::IdString("/Node")).getReleaseText());

		//Input texture
		nodeDef->addTextureSourceName("WindowRT", 0, Ogre::TextureDefinitionBase::TEXTURE_INPUT);
		nodeDef->setNumTargetPass(1);
		{
			Ogre::CompositorTargetDef* targetDef = nodeDef->addTargetPass("WindowRT");
			targetDef->setNumPasses(2 * viewports.size());
			{
				for(OgViewport* viewport : viewports)
				{
					{
						Ogre::CompositorPassClearDef* passClear = static_cast<Ogre::CompositorPassClearDef*>(targetDef->addPass(Ogre::PASS_CLEAR));
						passClear->mColourValue = Ogre::ColourValue(0.3f, 0.3f, 0.3f);
						passClear->mVpWidth = viewport->width() / target->getWidth();
						passClear->mVpHeight = viewport->height() / target->getHeight();
						passClear->mVpLeft = viewport->left() / target->getWidth();
						passClear->mVpTop = viewport->top() / target->getHeight();
					}
					{
						Ogre::CompositorPassSceneDef* passScene = static_cast<Ogre::CompositorPassSceneDef*>(targetDef->addPass(Ogre::PASS_SCENE));
						//passScene->mShadowNode = Ogre::IdString();
						passScene->mVpWidth = viewport->width() / target->getWidth();
						passScene->mVpHeight = viewport->height() / target->getHeight();
						passScene->mVpLeft = viewport->left() / target->getWidth();
						passScene->mVpTop = viewport->top() / target->getHeight();
						passScene->mCameraName = viewport->ogreCamera()->getName();
					}
				}
			}
		}

		Ogre::CompositorWorkspaceDef* workDef = compositorManager->addWorkspaceDefinition("MultiViewportWorkspace");
		workDef->connectOutput(nodeDef->getName(), 0);

		for(OgViewport* viewport : viewports)
			compositorManager->addWorkspace(viewport->sceneManager(), target, viewport->ogreCamera(), "MultiViewportWorkspace", true);
	}

	void OgreModule::setupHiddenWindow()
	{
		Ogre::NameValuePairList params;
		params["hidden"] = "true";

		Ogre::RenderWindow* window = mOgreRoot->createRenderWindow("mkHidden", 1, 1, false, &params);
	}

	void OgreModule::setupFirstRenderer()
	{
		mOgreRoot->setRenderSystem(mOgreRoot->getAvailableRenderers().at(0));
	}

	void OgreModule::setupRenderer(const string& name)
	{
		for(auto renderer : mOgreRoot->getAvailableRenderers())
			if(renderer->getName() == name)
			{
				mOgreRoot->setRenderSystem(renderer);
				return;
			}

		this->setupFirstRenderer();
	}

	unique_ptr<OgreWindow> OgreModule::createWindow(UiWindow* window, const string& name, int width, int height, bool fullScreen)
	{
		Ogre::NameValuePairList params;
		params["border"] = "none";

		return make_unique<OgreWindow>(this, window, mOgreRoot->createRenderWindow(name, width, height, fullScreen, &params));
	}

	void OgreModule::nextFrame()
	{
        Ogre::WindowEventUtilities::messagePump();

		if(mContextActive)
			mOgreRoot->renderOneFrame();
		else
			mOgreRoot->clearEventTimes();
	}

	void OgreModule::loadResources()
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}

    void OgreModule::setupResources()
    {
        // Load resource paths from config file
        Ogre::ConfigFile cf;
        cf.load(mResourcePath + "resources.cfg");

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
                // OS X does not set the working directory relative to the app,
                // In order to make_unique entities portable on OS X we need to provide
                // the loading with it's own bundle path location
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    Ogre::String(macBundlePath() + "/" + archName), typeName, secName);
#else
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
#endif
            }
        }
    }
}
