//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkSoundManager.h>

/* OpenAL */
#include <al.h>
#include <alc.h>

#include <Sound/mkSoundCallback.h>
#include <Sound/mkStaticSound.h>
#include <Sound/mkStreamSound.h>
#include <Sound/mkSharedBuffer.h>
#include <Sound/mkSoundListener.h>

#include <Object/Util/mkTimer.h>
#include <Sound/mkSoundManager.h>

#include <iostream>
#include <fstream>

namespace mk
{
	SoundManager::SoundManager()
		: mDevices()
		, mDevice(0)
		, mContext(0)
		, mVolume(1.f)
		, mGlobalPitch(1.f)
		, mActiveSounds()
		, mInactiveSounds()
		, mPausedSounds()
		, mLastActive()
		, mUpdateQueue()
		, mMaxSources(100)
		, mSourcePool()
		, mSharedBuffers()
		, mSoundListener(0)
		, mActions(0)
		, mDelayedActions(0)
		, mUpdateThread(0)
		, mShuttingDown(false)
	{}

	SoundManager::~SoundManager()
	{
		mShuttingDown = true;
		//mUpdateThread->join();
		//delete mUpdateThread;

		delete mActions;
		delete mDelayedActions;

		releaseAll();

		alcMakeContextCurrent(0);
		alcDestroyContext(mContext);
		alcCloseDevice(mDevice);

		delete mSoundListener;
	}

	void SoundManager::clearSources()
	{
		alDeleteSources(mMaxSources, &mSourcePool[0]);

		mSourcePool.clear();
	}

	void SoundManager::clearSounds()
	{
		for (SoundList::iterator it = mActiveSounds.begin(); it != mActiveSounds.end(); ++it)
		{
			destroySoundImpl((*it));
		}

		mPausedSounds.clear();
		mActiveSounds.clear();
		mInactiveSounds.clear();
	}

	void SoundManager::clearBuffers()
	{
		for(SharedBufferList::iterator it = mSharedBuffers.begin(); it != mSharedBuffers.end(); ++it)
		{
			delete (*it).second;
		}

		mSharedBuffers.clear();
	}

	void SoundManager::releaseAll()
	{
		stopAllSoundsImpl();
		clearSounds();
		clearBuffers();
		clearSources();
	}

	bool SoundManager::init(const string& deviceName, unsigned int maxSources, unsigned int queueListSize)
	{
		if (mDevice)
			return true;

		alGetError();

		enumDevices();

		if(deviceName != "" && std::find(mDevices.begin(), mDevices.end(), deviceName) != mDevices.end())
			mDevice = alcOpenDevice(deviceName.c_str());
		else if (mDevices.size() > 0)
			mDevice = alcOpenDevice(mDevices.front().c_str());
		else
			mDevice = alcOpenDevice(NULL);

		if (!mDevice)
			return false;
		

        ALint attribs[1] = {4};

		mContext = alcCreateContext(mDevice, attribs);
		if (!mContext)
			return false;


		if (!alcMakeContextCurrent(mContext))
			return false;

		//logFeatureSupport();

		mSoundListener = new/*sound*/SoundListener();

		mMaxSources = createSourcePool(maxSources);

		//mUpdateThread = new/*sound*/std::thread(std::bind(&SoundManager::threadUpdate, this));

		mActions = new/*sound*/LocklessQueue<SoundAction>(queueListSize);
		mDelayedActions = new/*sound*/LocklessQueue<SoundAction>(500);

		return true;
	}

	int SoundManager::createSourcePool(int maxSources)
	{
		ALuint source;
		int numSources = 0;

		while(alGetError() == AL_NO_ERROR && numSources < maxSources)
		{
			source = 0;
			alGenSources(1,&source);
			if(source != 0)
			{
				//std::cerr << "Adding Source : " << source << std::endl;
				mSourcePool.push_back(source);
				numSources++;
			}
			else
			{
				alGetError();
				break;
			}
		}

		return numSources;
	}

	void SoundManager::enumDevices()
	{
		const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

		/*
		** The list returned by the call to alcGetString has the names of the
		** devices seperated by NULL humans and the list is terminated by
		** two NULL humans, so we can cast the list into a string and it
		** will automatically stop at the first NULL that it sees, then we
		** can move the pointer ahead by the lenght of that string + 1 and we
		** will be at the begining of the next string.  Once we hit an empty
		** string we know that we've found the double NULL that terminates the
		** list and we can stop there.
		*/
		while(*deviceList != 0)
		{
			ALCdevice* device = alcOpenDevice(deviceList);
			if (alcGetError(device))
			{
				continue;
			}


			ALCcontext* context = alcCreateContext(device, NULL);
			if (alcGetError(device))
			{
				continue;
			}


			alcMakeContextCurrent(context);
			if(alcGetError(device))
			{
				continue;
			}

			mDevices.push_back(alcGetString(device, ALC_DEVICE_SPECIFIER));
						
			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);

			alcCloseDevice(device);

			deviceList += strlen(deviceList) + 1;
		}
	}

	void SoundManager::setMasterVolume(ALfloat vol)
	{
		mVolume = vol;
		alListenerf(AL_GAIN, vol);
	}

	Sound* SoundManager::createSound(const string& fileName, bool loop, bool stream, SoundCallback* callback)
	{
		std::ifstream ifile(fileName.c_str(), std::ifstream::out);
		if(!ifile.is_open())
			return 0;

		Sound* sound = 0;

		if(stream)
			sound = new/*sound*/StreamSound(this, callback);
		else
			sound = new/*sound*/StaticSound(this, callback);

		sound->setLoop(loop);

		addAction(std::bind(&SoundManager::createSoundImpl, this, sound, fileName, stream));

		return sound;
	}

	void SoundManager::createSoundImpl(Sound* sound, const string& fileName, bool stream)
	{
		//std::cerr << "creating sound Impl : " << fileName << std::endl;
		mInactiveSounds.push_back(sound);

		if(!stream)
			sound->openShared(getSharedBuffer(fileName));
		else
			sound->open(fileName);

		//std::cerr << "done" << std::endl;
	}

	void SoundManager::stopAllSounds()
	{
		addAction(std::bind(&SoundManager::stopAllSoundsImpl, this));
	}

	void SoundManager::setGlobalPitch(float pitch)
	{
		mGlobalPitch = pitch;

		addAction(std::bind(&SoundManager::setGlobalPitchImpl, this));
	}

	void SoundManager::pauseAllSounds()
	{
		addAction(std::bind(&SoundManager::pauseAllSoundsImpl, this));
	}

	void SoundManager::resumeAllSounds()
	{
		addAction(std::bind(&SoundManager::resumeAllSoundsImpl, this));
	}

	void SoundManager::muteAllSounds()
	{
		alGetListenerf(AL_GAIN, &mVolume);
		alListenerf(AL_GAIN, 0.f);
	}

	void SoundManager::unmuteAllSounds()
	{
		alListenerf(AL_GAIN, mVolume);
	}

	void SoundManager::destroySound(Sound* sound)
	{
		addAction(std::bind(&SoundManager::destroySoundImpl, this, sound));
	}

	void SoundManager::setDistanceModel(ALenum value)
	{
		alDistanceModel(value);
	}

	void SoundManager::setSpeedOfSound(float speed)
	{
		alSpeedOfSound(speed);
	}

	void SoundManager::setDopplerFactor(float factor)
	{
		alDopplerFactor(factor);
	}

	void SoundManager::updateActiveSound(Sound* sound)
	{
		releaseActiveSound(sound);
		queueActiveSound(sound);
	}

	void SoundManager::queueActiveSound(Sound* sound)
	{
		unsigned int index = 0;
		SoundList::iterator pos = mActiveSounds.begin();

		while(pos != mActiveSounds.end() && (*pos)->getPriority() < sound->getPriority()) { ++index; ++pos; }

		if(mActiveSounds.size() < mMaxSources)
		{
			sound->assignSource(mSourcePool.back());
			mSourcePool.pop_back();
		}
		else if(index <= mMaxSources)
		{
			sound->assignSource((*mLastActive)->getSource());
			(*mLastActive)->releaseSource();
			--mLastActive;
		}

		mActiveSounds.insert(pos, sound);

		sound->getPriority().mQueueIndex = index;
		sound->getPriority().mQueueIterator = pos;
	}

	void SoundManager::activateSound(Sound* sound)
	{
		queueActiveSound(sound);
		mInactiveSounds.remove(sound);
	}

	void SoundManager::releaseActiveSound(Sound* sound)
	{
		int lastIndex = sound->getPriority().mQueueIndex;
		SoundList::iterator lastPos = sound->getPriority().mQueueIterator;

		if(mActiveSounds.size() <= mMaxSources)
		{
			mSourcePool.push_back(sound->getSource());
			sound->releaseSource();
		}
		else
		{
			++mLastActive;
			(*mLastActive)->assignSource(sound->getSource());
			sound->releaseSource();
		}

		mActiveSounds.remove(sound);
	}

	void SoundManager::disactivateSound(Sound* sound)
	{
		releaseActiveSound(sound);
		mInactiveSounds.push_back(sound);
	}

	void SoundManager::updatePosition(Sound* sound, const btVector3& position)
	{
		sound->getPriority().mDistance = position.distance(mSoundListener->position());
		mUpdateQueue.push_back(sound);
	}

	void SoundManager::destroySoundImpl(Sound* sound)
	{
		//std::cerr << "destroying sound " << std::endl;
		if(sound->isActive())
			releaseActiveSound(sound);
		else
			mInactiveSounds.remove(sound);

		if(sound->isPaused())
			mPausedSounds.remove(sound);

		delete sound;
	}

	void SoundManager::playSound(Sound* sound)
	{
		addAction(std::bind(&SoundManager::playSoundImpl, this, sound));
	}

	void SoundManager::pauseSound(Sound* sound)
	{
		addAction(std::bind(&SoundManager::pauseSoundImpl, this, sound));
	}

	void SoundManager::stopSound(Sound* sound)
	{
		addAction(std::bind(&SoundManager::stopSoundImpl, this, sound));
	}

	void SoundManager::playSoundImpl(Sound* sound)
	{
		activateSound(sound);
		sound->playImpl();
	}

	void SoundManager::pauseSoundImpl(Sound* sound)
	{
		sound->pauseImpl();
		mPausedSounds.push_back(sound);
		disactivateSound(sound);
	}

	void SoundManager::stopSoundImpl(Sound* sound)
	{
		sound->stopImpl();
		disactivateSound(sound);

		if(sound->isTemporary())
			destroySound(sound);
	}

	void SoundManager::stopAllSoundsImpl()
	{
		SoundList activeSounds(mActiveSounds.begin(), mActiveSounds.end());

		for(Sound* sound : activeSounds)
			stopSoundImpl(sound);
	}

	void SoundManager::pauseAllSoundsImpl()
	{
		SoundList activeSounds(mActiveSounds.begin(), mActiveSounds.end());

		for(Sound* sound : activeSounds)
			pauseSoundImpl(sound);
	}

	void SoundManager::resumeAllSoundsImpl()
	{
		SoundList pausedSounds(mPausedSounds.begin(), mPausedSounds.end());

		for(Sound* sound : pausedSounds)
			playSoundImpl(sound);
	}

	void SoundManager::setGlobalPitchImpl()
	{
		for(Sound* sound : mActiveSounds)
			sound->setPitch(mGlobalPitch);
	}

	SharedBuffer* SoundManager::createSharedBuffer(const string& fileName)
	{
		SharedBuffer* buffer = new/*sound*/SharedBuffer(fileName, std::bind(&SoundManager::releaseBuffer, this, std::placeholders::_1));
		mSharedBuffers[fileName] = buffer;

		return buffer;
	}

	SharedBuffer* SoundManager::getSharedBuffer(const string& fileName)
	{
		SharedBufferList::const_iterator find = mSharedBuffers.find(fileName);

		if(find != mSharedBuffers.end())
			return (*find).second;
		else
			return createSharedBuffer(fileName);
	}

	void SoundManager::releaseBuffer(SharedBuffer* buffer)
	{
		mSharedBuffers.erase(mSharedBuffers.find(buffer->getFileName()));
		delete buffer;
	}

	void SoundManager::addAction(const SoundAction& action)
	{
		if(mDelayedActions->empty())
		{
			if(!mActions->push(action))
				mDelayedActions->push(action);
		}
		else
		{
			mDelayedActions->push(action);
		}
	}

	void SoundManager::processActions()
	{
		int i = 0;
		std::function<void ()> action;

		if(!mActions->empty())
		{
			while(((++i) < 5) && mActions->pop(action))
			{
				action();
			}
		}

		if(!mDelayedActions->empty())
		{
			while(((++i) < 5) && mDelayedActions->pop(action))
			{
				action();
			}
		}
	}

	void SoundManager::updateSounds()
	{
		double timeStep = Timer::me()->read(this);

		//static Ogre::uint32 cTime=0;
		//static Ogre::uint32 pTime=0;
		//static Ogre::Timer timer;
		//static float rTime=0.f;

		// Get frame time
		//cTime = timer.getMillisecondsCPU();
		//float fTime = (cTime-pTime) * 0.001f;

		mSoundListener->update();

		int i = 0;
		for(SoundList::const_iterator it = mActiveSounds.begin(); i != mMaxSources && it != mActiveSounds.end(); ++it)
		{
			(*it)->update(timeStep);
			++i;
		}

		//pTime = cTime;
		Timer::me()->update(this);
	}

	void SoundManager::threadUpdate()
	{
		//std::cerr << "SoundManager::nextFrame" << std::endl;
		//while(!mShuttingDown)
		//{
			updateSounds();
			processActions();
			//std::this_thread::sleep(std::posix_time::milliseconds(10));
		//}
		//std::cerr << "SoundManager::nextFrame" << std::endl;
	}

	void SoundManager::logFeatureSupport()
	{
		/*mLogger->logMessage("*** --- SUPPORTED FORMATS");
		ALenum eBufferFormat = 0;


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_MONO16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_MONO16 -- Monophonic Sound");


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_STEREO16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_STEREO16 -- Stereo Sound");


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_QUAD16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_QUAD16 -- 4 Channel Sound");


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_51CHN16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_51CHN16 -- 5.1 Surround Sound");


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_61CHN16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_61CHN16 -- 6.1 Surround Sound");


		eBufferFormat = alcGetEnumValue(mDevice, "AL_FORMAT_71CHN16");
		if(eBufferFormat)
			mLogger->logMessage("*** --- AL_FORMAT_71CHN16 -- 7.1 Surround Sound");*/
	}
}
