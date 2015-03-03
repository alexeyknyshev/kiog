//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkSoundListener.h>

#include <al.h>
#include <alc.h>

namespace mk
{
	SoundListener::SoundListener()
		: mTransformUpdated(false)
		, mPosition(0,0,0)
	{}

	SoundListener::~SoundListener()
	{}

	void SoundListener::setTransform(const btVector3& position, const btVector3& up, const btVector3& front)
	{
		mPosition = position;

		mOrientation[0] = front.x();
		mOrientation[1] = front.y();
		mOrientation[2] = front.z();
		mOrientation[3] = up.x();
		mOrientation[4] = up.y();
		mOrientation[5] = up.z();

		mTransformUpdated = true;
	}

	void SoundListener::setTransform(const btVector3& position, const btQuaternion& rotation)
	{
		mOrientation = rotation;

		// this->setTransform(position, xxx, xxx);
	}

	void SoundListener::update()
	{
		if(mTransformUpdated)
		{
			alListener3f(AL_POSITION, mPosition.x(), mPosition.y(), mPosition.z());
			alListenerfv(AL_ORIENTATION, mOrientation);

			//alListener3f(AL_VELOCITY, mEntity->direction().getX(), mEntity->direction().getY(), mEntity->direction().getZ());

			mTransformUpdated = false;
		}

        
	}
}
