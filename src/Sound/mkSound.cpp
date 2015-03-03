//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkSound.h>

#include <Sound/mkSharedBuffer.h>
#include <Sound/mkSoundCallback.h>
#include <Sound/mkSoundImplementer.h>

#include <iostream>

namespace mk
{
	bool operator < (Sound::Priority& rhs, Sound::Priority& lhs)
	{
		if(rhs.mLevel > lhs.mLevel)
			return true;
		else if(rhs.mLevel < lhs.mLevel)
			return false;

		else if(rhs.mDistance < lhs.mDistance)
				return true;
		else if(rhs.mDistance > lhs.mDistance)
				return false;

		else return false;
	}

	Sound::Sound(SoundImplementer* manager, SoundCallback* callback)
		: mManager(manager)
		, mCallback(callback)
		, mSource(0)
		, mPriority()
		, mGain(1.0f)
		, mPitch(1.0f)
		, mLoop(false)
		, mTemporary(false)
		, mStream(false)
		, mState(STOPPED)
		, mCursor(0.f)
		, mDuration(0.0f) 
		, mSeekable(true)
		, mPosition(0,0,0)
		, mDirection(0,0,0)
		, mVelocity(0,0,0)
		, mMaxDistance(1E10) 
		, mMaxGain(1.0f) 
		, mMinGain(0.0f)
		, mReferenceDistance(1.0f)
		, mRolloffFactor(1.0f) 
		, mInnerConeAngle(360.0f) 
		, mOuterConeAngle(360.0f) 
		, mOuterConeGain(0.0f) 
		, mUpdateCursor(false)
		, mUpdateTransform(false)
		, mFadeTimer(0.0f) 
		, mFadeTime(1.0f) 
		, mFadeInitVol(0) 
		, mFadeEndVol(1) 
		, mFade(false) 
		, mFadeEndAction(FC_NONE)  
		, mCuePoints()
	{}

	Sound::~Sound() 
	{
		if(mCallback)
			mCallback->soundDestroyed(this);

		release();
	}

	void Sound::update(float fTime)
	{
		if(!mActive || mState != PLAYING)
			return;

		ALenum state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);

		if (state == AL_STOPPED)
		{
			stop();
			return;
		}

		if(mUpdateTransform)
		{
			alSource3f(mSource, AL_POSITION, mPosition.getX(), mPosition.getY(), mPosition.getZ());
			alSource3f(mSource, AL_DIRECTION, mDirection.getX(), mDirection.getY(), mDirection.getZ());
			mUpdateTransform = false;
		}

		if(mUpdateCursor)
		{
			updatePlayCursor();
		}

		updateBuffers();
		updateFade(fTime);
	}

	void Sound::checkError()
	{
		alGetError();
	}

	void Sound::play()
	{
		mManager->playSound(this);
	}

	void Sound::stop()
	{
		mManager->stopSound(this);
	}
	void Sound::pause()
	{
		mManager->pauseSound(this);
	}

	void Sound::playImpl()
	{
		if(mState == PLAYING)
			return;

		mState = PLAYING;

		if(mActive)
		{
			updatePlayCursor();

			clearBuffers();
			fillBuffers();

			alSourcePlay(mSource);
		}
	}

	void Sound::stopImpl()
	{
		if (mState == STOPPED)
			return;

		mState = STOPPED;

		if(mActive)
		{
			alSourceStop(mSource);

			clearBuffers();
			rewind();
		}
	}

	void Sound::pauseImpl()
	{
		mState = PAUSED;

		if(mActive)
		{
			mCursor = getPlayCursor();
			alSourcePause(mSource);
		}
	}

	void Sound::setPlayCursor(float seconds)
	{
		mCursor = seconds;
		mUpdateCursor = true;
	}

	void Sound::disable3D()
	{
		mSourceRelative = false;
		mPosition = btVector3(0,0,0);
		mDirection = btVector3(0,0,0);
		mReferenceDistance = 1.f;

		if(mActive)
		{
			alSourcei (mSource, AL_SOURCE_RELATIVE, mSourceRelative);
			alSourcef (mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
			alSource3f(mSource, AL_POSITION, mPosition.getX(), mPosition.getY(), mPosition.getZ());
			alSource3f(mSource, AL_DIRECTION, mDirection.getX(), mDirection.getY(), mDirection.getZ());
		}
	}

	void Sound::enable3D()
	{
		mSourceRelative = false;

		if(mActive)
		{
			alSourcei (mSource, AL_SOURCE_RELATIVE, mSourceRelative);
			alSourcef (mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
			alSource3f(mSource, AL_POSITION, mPosition.getX(), mPosition.getY(), mPosition.getZ());
			alSource3f(mSource, AL_DIRECTION, mDirection.getX(), mDirection.getY(), mDirection.getZ());
		}
	}

	void Sound::setPosition(const btVector3& pos)
	{
		mPosition = pos;
		mUpdateTransform = true;
	}

	void Sound::setDirection(const btVector3& dir)
	{
		mDirection = dir;
		mUpdateTransform = true;
	}

	void Sound::setVelocity(const btVector3& vel)
	{
		mVelocity = vel;

		if(mActive)
			alSource3f(mSource, AL_VELOCITY, vel.getX(), vel.getY(), vel.getZ());
	}

	void Sound::setLoop(bool loop)
	{
		mLoop = loop;
	}

	void Sound::setVolume(float gain)
	{
		mGain = gain;

		if(mActive)
			alSourcef(mSource, AL_GAIN, mGain);
	}

	void Sound::setMaxVolume(float maxGain)
	{
		mMaxGain = maxGain;

		if(mActive)
			alSourcef(mSource, AL_MAX_GAIN, mMaxGain);
	}

	void Sound::setMinVolume(float minGain)
	{
		mMinGain = minGain;

		if(mActive)
			alSourcef(mSource, AL_MIN_GAIN, mMinGain);
	}

	void Sound::setConeAngles(float insideAngle, float outsideAngle)
	{
		mInnerConeAngle = insideAngle;
		mOuterConeAngle = outsideAngle;

		if(mActive)
		{
			alSourcef (mSource, AL_CONE_INNER_ANGLE, mInnerConeAngle);
			alSourcef (mSource, AL_CONE_OUTER_ANGLE, mOuterConeAngle);
		}
	}

	void Sound::setOuterConeVolume(float gain)
	{
		mOuterConeGain = gain;

		if(mActive)
			alSourcef (mSource, AL_CONE_OUTER_GAIN, mOuterConeGain);
	}

	void Sound::setMaxDistance(float maxDistance)
	{
		mMaxDistance = maxDistance;

		if(mActive)
			alSourcef(mSource, AL_MAX_DISTANCE, mMaxDistance);
	}

	void Sound::setRolloffFactor(float rolloffFactor)
	{
		mRolloffFactor = rolloffFactor;

		if(mActive)
			alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);		
	}

	void Sound::setReferenceDistance(float referenceDistance)
	{
		mReferenceDistance = referenceDistance;

		if(mActive)
			alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);		
	}

	void Sound::setPitch(float pitch)
	{
		mPitch = pitch;

		if(mActive)
			alSourcef(mSource, AL_PITCH, mPitch);		
	}

	void Sound::initSource()
	{		
		alSourcef (mSource, AL_GAIN,				mGain);
		alSourcef (mSource, AL_MAX_GAIN,			mMaxGain);
		alSourcef (mSource, AL_MIN_GAIN,			mMinGain);
		alSourcef (mSource, AL_MAX_DISTANCE,		mMaxDistance);	
		alSourcef (mSource, AL_ROLLOFF_FACTOR,		mRolloffFactor);
		alSourcef (mSource, AL_REFERENCE_DISTANCE,	mReferenceDistance);
		alSourcef (mSource, AL_CONE_OUTER_GAIN,		mOuterConeGain);
		alSourcef (mSource, AL_CONE_INNER_ANGLE,	mInnerConeAngle);
		alSourcef (mSource, AL_CONE_OUTER_ANGLE,	mOuterConeAngle);
		alSource3f(mSource, AL_POSITION,			mPosition.getX(), mPosition.getY(), mPosition.getZ());
		alSource3f(mSource, AL_DIRECTION,			mDirection.getX(), mDirection.getY(), mDirection.getZ());
		alSource3f(mSource, AL_VELOCITY,			mVelocity.getX(), mVelocity.getY(), mVelocity.getZ());
		alSourcef (mSource, AL_PITCH,				mPitch);
		alSourcei (mSource, AL_SOURCE_RELATIVE,		mSourceRelative);
		alSourcei (mSource, AL_LOOPING,				mLoop);
		alSourcei (mSource, AL_SOURCE_STATE,		AL_INITIAL);
	}

	void Sound::assignSource(ALuint src)
	{
		mActive = true;
		mSource = src;
		initSource();
		fillBuffers();	

		if(mPauseOnDisactivate)
			playImpl();
	}

	void Sound::releaseSource()
	{
		if(mPauseOnDisactivate)
			pauseImpl();

		alSourceStop(mSource);
		clearBuffers();	

		mSource = AL_NONE;
		mActive = false;
	}

	void Sound::startFade(bool fDir, float fadeTime, FadeControl actionOnComplete)
	{
		mFade = true;
	    mFadeInitVol = fDir ? 0.f : getVolume();
		mFadeEndVol = fDir ? 1.f : 0.f;
		mFadeTimer = 0.f;
		mFadeEndAction = actionOnComplete;
		mFadeTime = fadeTime;

		if(fDir = true && !isPlaying())
			play();
	}

	void Sound::updateFade(float fTime)
	{
		if (mFade)
		{
			if((mFadeTimer+=fTime) >= mFadeTime)
			{
				setVolume(mFadeEndVol);
				mFade = false;

				switch(mFadeEndAction) 
				{
				case FC_PAUSE: 
					pause(); 
					break;
				case FC_STOP: 
					stop(); 
					break;
				}
			}
			else
			{
				float vol = (mFadeEndVol-mFadeInitVol)*(mFadeTimer/mFadeTime);
				setVolume(mFadeInitVol + vol);
			}
		} 
	}
}
