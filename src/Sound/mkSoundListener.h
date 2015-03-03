//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_SOUNDLISTENER_H_INCLUDED
#define MK_SOUNDLISTENER_H_INCLUDED

/* bullet */
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

namespace mk
{
	class MK_SOUND_EXPORT SoundListener
	{
	public:
		BT_DECLARE_ALIGNED_ALLOCATOR();

	public:
		SoundListener();
		~SoundListener();

		void setTransform(const btVector3& position, const btQuaternion& rotation);
		void setTransform(const btVector3& position, const btVector3& front, const btVector3& up);

		const btVector3& position() const { return mPosition; }
		const btQuaternion& orientation() const { return mOrientation; }

		void update();

	private:
		bool mTransformUpdated;
		btVector3 mPosition;
		btQuaternion mOrientation;
		float mOrientationArray[6];
	};
}

#endif // MK_SOUNDLISTENER_H_INCLUDED
