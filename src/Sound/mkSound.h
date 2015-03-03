//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_ISOUND_H_INCLUDED
#define MK_ISOUND_H_INCLUDED

/* mk headers */
#include <Sound/mkSoundForward.h>

/* bullet*/
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

/* standard */
#include <list>
#include <functional>
#include <stdint.h>

namespace mk
{
	class MK_SOUND_EXPORT Sound
	{
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();

	public:
		enum State
		{
			PLAYING,
			PAUSED,
			STOPPED
		};

		struct Priority
		{
			uint8_t mLevel;
			float mDistance;

			int mQueueIndex;
			std::list<Sound*>::iterator mQueueIterator;

			friend bool operator < (Priority& rhs, Priority& lhs);
		};

		enum FadeControl
		{
			FC_NONE		= 0x00,
			FC_PAUSE	= 0x01,
			FC_STOP		= 0x02
		};

	// Thread-Safe externals
	public:
		Sound(SoundImplementer* manager, SoundCallback* callback = NULL);
		virtual ~Sound();

		void play();
		void stop();
		void pause();

		// @check : are these thread-safe ???
		void setDirection(const btVector3& dir);
		void setPosition(const btVector3& pos);
		void setVelocity(const btVector3& vel);

	// Thread-Exclusive externals
	public:
		void update(float fTime);

		void playImpl();
		void stopImpl();
		void pauseImpl();

		void enable3D();
		void disable3D();

		virtual void setLoop(bool loop);

		void setVolume(float gain);
		void setMaxVolume(float maxGain);
		void setMinVolume(float minGain);

		void setConeAngles(float insideAngle, float outsideAngle);
		void setOuterConeVolume(float gain);
		void setMaxDistance(float maxDistance);
		void setRolloffFactor(float rolloffFactor);
		void setReferenceDistance(float referenceDistance);

		void setPitch(float pitch);

		void setPriority(Priority priority) { mPriority = priority; }

		void assignSource(ALuint src);
		void releaseSource();

		void setPlayCursor(float seconds);

		void startFade(bool fDir, float fadeTime, FadeControl actionOnComplete = FC_NONE);

		void markTemporary() { mTemporary = true; }

	public:
		inline State getState() const { return mState; }

		inline bool isPlaying() const { return mState == PLAYING; }
		inline bool isPaused() const { return mState == PAUSED; }
		inline bool isStopped() const { return mState == STOPPED; }

		inline bool isActive() const { return mActive; }

		inline float getVolume() const { return mGain; }
		inline float getPitch() const { return mPitch; }

		inline float getRolloffFactor() const { return mRolloffFactor; }
		inline float getMaxDistance() const { return mMaxDistance; }
		inline float getReferenceDistance() const { return mReferenceDistance; }

		inline const btVector3& position() const { return mPosition; }
		inline const btVector3& direction() const { return mDirection; }

		inline bool isFading() const { return mFade; }
		inline bool isTemporary() const { return mTemporary; }
		inline  bool isMono() const { return mMono; }

		inline ALuint getSource() const { return mSource; }
		inline float getDuration() const { return mDuration; }

		Priority& getPriority() { return mPriority; }

	public:
		virtual void open(const string& fileName) {}
		virtual void openShared(SharedBuffer* buffer) {}

	protected:
		virtual void release() {}

		virtual void fillBuffers() = 0;
		virtual void clearBuffers() = 0;
		virtual void updateBuffers() = 0;

		virtual void rewind() = 0;
		virtual void updatePlayCursor() = 0;
		virtual ALfloat getPlayCursor() = 0;

	protected:
		void initSource();

		void checkError();

		void updateFade(float fTime);

	protected:
		SoundImplementer* mManager;
		SoundCallback* mCallback;

		ALuint mSource;					// OpenAL Source
		Priority mPriority;				// Priority assigned to source
		bool mActive;
		bool mPauseOnDisactivate;

		float mGain;					// Current volume
		float mPitch;					// Current pitch 

		bool mLoop;
		bool mTemporary;
		bool mStream;					// Stream flag
		bool mMono;

		State mState;
		ALfloat mCursor;				// Playback position in seconds

		float mDuration;				// Time in seconds of sound file
		bool mSeekable;					// Flag indicating seeking available

		bool mSourceRelative;

		btVector3 mPosition;			// 3D position
		btVector3 mDirection;			// 3D direction
		btVector3 mVelocity;			// 3D velocity

		float mMaxGain;					// Minimum volume
		float mMinGain;					// Maximum volume
		float mMaxDistance;				// Maximum attenuation distance
		float mRolloffFactor;			// Rolloff factor for attenuation
		float mReferenceDistance;		// Half-volume distance for attenuation

		float mOuterConeGain;			// Outer cone volume
		float mInnerConeAngle;			// Inner cone angle
		float mOuterConeAngle;			// outer cone angle

		bool mUpdateTransform;				// Transformation update flag
		bool mUpdateCursor;					// Flag indicating playback position has changed

		std::list<float> mCuePoints;	// List of play position points

		/* Fade sound */
		float mFadeTimer;
		float mFadeTime;
		float mFadeInitVol;
		float mFadeEndVol;
		bool mFade;
		FadeControl mFadeEndAction;
	};
}

#endif // MK_ISOUND_H_INCLUDED
