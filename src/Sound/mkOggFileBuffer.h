//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_OGGFILEBUFFER_H_INCLUDED
#define MK_OGGFILEBUFFER_H_INCLUDED

/* mk headers */
#include <Sound/mkSoundFileBuffer.h>

/* Lib Vorbis */
#include <vorbisfile.h>

/* OpenAL */
#include <al.h>
#include <alc.h>

namespace mk
{
	class MK_SOUND_EXPORT OggFileBuffer : public SoundFileBuffer
	{
	public:	
		OggFileBuffer();
		~OggFileBuffer();

		void open(const string& fileName);
		void close();
		void reopen();

		void seekTime(float time);
		void seekBytes(long pos);

		void fill(ALuint buffer);
		bool fillChunk(ALuint buffer);

		ALenum getFormat() { return mFormat; }
		bool isSeekable() { return mSeekable; }
		bool isMono() { return mMono; }
		ALfloat getDuration() { return mDuration; }
		const string& getFileName() { return mFileName; }

	protected:
		bool getBufferInfo();		
		bool readChunk(char* chunk, size_t chunkSize);

	private:
		string mFileName;				// Name of audio file stream (Used with shared buffers)

		OggVorbis_File	mOggFile;			// OggVorbis file structure
		vorbis_info*	mVorbisInfo;		// Vorbis info 
		vorbis_comment* mVorbisComment;		// Vorbis comments

		ov_callbacks mOggCallbacks;

		size_t mChunkSize;					// Size of audio buffer (250ms)
		ALenum mFormat;						// OpenAL buffer format

		bool mSeekable;
		ALfloat mDuration;
		bool mMono;
	};
}

#endif // MK_OGGFILEBUFFER_H_INCLUDED
