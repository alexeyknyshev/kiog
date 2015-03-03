//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGREGEOMETRY_H_INCLUDED
#define MK_OGREGEOMETRY_H_INCLUDED

/* mk headers */
#include <Og/mkOgForward.h>
#include <OGRE/OgreMatrix4.h>

namespace mk
{
	MK_OG_EXPORT void getSubMeshGeometrySize(const Ogre::Mesh* const mesh, const Ogre::SubMesh* const submesh, size_t& vertex_count, size_t& index_count, bool& added_shared);
	MK_OG_EXPORT void getMeshGeometrySize(const Ogre::Mesh* const mesh, size_t &vertex_count, size_t &index_count);
	MK_OG_EXPORT void getMeshGeometry(const Ogre::Mesh* const mesh,
								float* vertices,
								int* indices,
								size_t& index_offset,
								size_t& vertex_offset,
								const Ogre::Matrix4 &transform = Ogre::Matrix4::IDENTITY	);

}

#endif // MK_OGREGEOMETRY_H_INCLUDED
