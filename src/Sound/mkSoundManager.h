//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDMANAGER_H_INCLUDED
#define MK_SOUNDMANAGER_H_INCLUDED

/* mk headers */
#include <Object/String/mkString.h>
#include <Object/Threading/mkLocklessQueue.h>
#include <Sound/mkSoundForward.h>
#include <Sound/mkSoundImplementer.h>

/* Basic headers */
#include <deque>
#include <list>
#include <vector>
#include <map>
#include <thread>
#include <functional>

namespace mk
{
	class MK_SOUND_EXPORT SoundManager : public SoundImplementer
	{
	public:
		typedef std::function<void ()> SoundAction;
		typedef std::vector<string> StringVector;
		typedef std::list<Sound*> SoundList;
		typedef std::vector<Sound*> SoundVector;
		typedef std::vector<ALuint> SourceVector;
		typedef std::map<string, SharedBuffer*> SharedBufferList;

	// Thread-safe interface
	public:
		SoundManager();
		~SoundManager();

		bool init(const string& deviceName = "", unsigned int maxSources = 100, unsigned int queueListSize = 64);

		Sound* createSound(const string& file, bool loop = false, bool stream = false, SoundCallback* callback = 0);

	public:
		void setMasterVolume(ALfloat vol);
		void setGlobalPitch(float pitch);

		void setDistanceModel(ALenum value);
		void setDopplerFactor(float factor = 1.f);
		void setSpeedOfSound(float speed = 363.f);

	public:
		inline const StringVector& getDeviceList() const { return mDevices; }

		inline ALfloat getMasterVolume() const { return mVolume; }
		inline float getGlobalPitch() const { return mGlobalPitch; }
		inline SoundListener* getListener() { return mSoundListener; }

		inline int getMaxSources() const { return mMaxSources; }

	public:
		void playSound(Sound* sound);
		void stopSound(Sound* sound);
		void pauseSound(Sound* sound);
		void destroySound(Sound* sound);

		void updatePosition(Sound* sound, const btVector3& position);

		void stopAllSounds();
		void pauseAllSounds();
		void muteAllSounds();
		void unmuteAllSounds();
		void resumeAllSounds();

	private:
		void enumDevices();
		int createSourcePool(int numSources);

		void clearSounds();
		void clearSources();
		void clearBuffers();
		void releaseAll();

	// Thread-safe implementation to be executed by one same unique thread
	public:
		void threadUpdate();

		void createSoundImpl(Sound* sound, const string& fileName, bool stream);
		void destroySoundImpl(Sound* sound);

		void playSoundImpl(Sound* sound);
		void stopSoundImpl(Sound* sound);
		void pauseSoundImpl(Sound* sound);

		void setGlobalPitchImpl();

		void stopAllSoundsImpl();
		void pauseAllSoundsImpl();
		void resumeAllSoundsImpl();

	private:
		void updateActiveSound(Sound* sound);
		void releaseActiveSound(Sound* sound);
		void queueActiveSound(Sound* sound);
		
		void activateSound(Sound* sound);
		void disactivateSound(Sound* sound);

		SharedBuffer* createSharedBuffer(const string& fileName);
		SharedBuffer* getSharedBuffer(const string& fileName);
		void releaseBuffer(SharedBuffer* buffer);

		void logFeatureSupport();

	private:
		void addAction(const SoundAction& action);

		void processActions();
		void updateSounds();

	private:
		LocklessQueue<SoundAction>* mActions;
		LocklessQueue<SoundAction>* mDelayedActions;

		std::thread* mUpdateThread;
		bool mShuttingDown;

	private:
		StringVector mDevices;					// List of available devices strings
		ALCdevice* mDevice;						// OpenAL device
		ALCcontext* mContext;					// OpenAL context

		ALfloat	mVolume;						// Main Volume
		float mGlobalPitch;						// Global pitch modifier

		/** Sounds **/
		SoundList mActiveSounds;				// list of all sounds : mActiveSounds.begin() to mActiveSounds[mMaxSources] are active
		SoundList mInactiveSounds;
		SoundList mPausedSounds;				// list of sounds currently paused

		SoundList::iterator mLastActive;
		SoundVector mUpdateQueue;

		unsigned int mMaxSources;				// Maximum Number of sources to allocate
		SourceVector mSourcePool;				// List of available sources

		SharedBufferList mSharedBuffers;		// List of shared static buffers

		SoundListener* mSoundListener;			// Listener object
	};
}

#endif // MK_SOUNDMANAGER_H_INCLUDED
