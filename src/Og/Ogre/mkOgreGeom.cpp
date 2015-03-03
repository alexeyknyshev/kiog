//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Og/mkOgConfig.h>
#include <Og/Ogre/mkOgreGeom.h>

#include <Og/Ogre/mkOgreModule.h>

#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSubmesh.h>

namespace mk
{
	void getSubMeshGeometrySize(const Ogre::Mesh* const mesh, const Ogre::SubMesh* const submesh, size_t& vertex_count, size_t& index_count, bool& added_shared)
	{
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}
		// Add the indices
		index_count += submesh->indexData->indexCount;
	}

	void getMeshGeometrySize(const Ogre::Mesh* const mesh, size_t& vertex_count, size_t& index_count)
	{
		bool added_shared = false;

		// Calculate how many vertices and indices we're going to need
		for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);
			// We only need to add the shared vertices once
			if(submesh->useSharedVertices)
			{
				if( !added_shared )
				{
					vertex_count += mesh->sharedVertexData->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				vertex_count += submesh->vertexData->vertexCount;
			}
			// Add the indices
			index_count += submesh->indexData->indexCount;
		}
	}

	void getMeshGeometry(	const Ogre::Mesh* const mesh,
										float* vertices,
										int* indices,
										size_t& index_offset,
										size_t& vertex_offset,
										const Ogre::Matrix4 &transform)
	{
		bool added_shared = false;
		size_t& current_offset = vertex_offset;
		size_t shared_offset = vertex_offset;
		size_t next_offset = vertex_offset;

		//std::cerr << "getMeshGeometry" << index_offset << "," << vertex_offset << std::endl;

		// Run through the submeshes again, adding the data into the arrays
		for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);

			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

			if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
			{
				if(submesh->useSharedVertices)
				{
					added_shared = true;
					shared_offset = current_offset;
				}

				const Ogre::VertexElement* posElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

				Ogre::HardwareVertexBufferSharedPtr vbuf =
					vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

				unsigned char* vertex =
					static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

				// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
				//  as second argument. So make_unique it float, to avoid trouble when Ogre::Real will
				//  be comiled/typedefed as double:
				//Ogre::Real* pReal;
				float* pReal;
				size_t vertsIndex = current_offset*3;

				for( size_t j = 0; j < vertex_data->vertexCount; j++, vertex += vbuf->getVertexSize())
				{
					//std::cerr << "adding a vertex : index = " << vertsIndex << std::endl;
					posElem->baseVertexPointerToElement(vertex, &pReal);
					Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

					pt = transform * pt;

					vertices[vertsIndex++] = pt.x;
					vertices[vertsIndex++] = pt.y;
					vertices[vertsIndex++] = pt.z;
				}

				vbuf->unlock();
				next_offset += vertex_data->vertexCount;
			}

			Ogre::IndexData* index_data = submesh->indexData;
			size_t numTris = index_data->indexCount / 3;
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

			unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

			size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			if ( use32bitindexes )
			{
				for ( size_t k = 0; k < numTris*3; ++k)
				{
					indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
				}
			}
			else
			{
				for ( size_t k = 0; k < numTris*3; ++k)
				{
					indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
						static_cast<unsigned long>(offset);
				}
			}

			ibuf->unlock();
			current_offset = next_offset;
		}
	}
}
