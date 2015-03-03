//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Sound/mkSoundConfig.h>
#include <Sound/mkOggFileBuffer.h>

#include <iostream>
#include <vector>

namespace mk
{
	size_t oggRead(void* ptr, size_t size, size_t count, void* file)
	{
		return fread (ptr, size, count, (FILE*)file);
	}

	int oggSeek(void* file, ogg_int64_t offset, int whence)
	{
		return fseek((FILE*)file, offset, whence);
	}

	int oggClose(void* file)
	{
		return fclose((FILE*)file);
	}

	long oggTell(void* file)
	{
		return ftell((FILE*)file);
	}


	OggFileBuffer::OggFileBuffer()
		: mFileName("")
		, mVorbisInfo(0)
		, mVorbisComment(0)
		, mChunkSize(0)
		, mFormat()
		, mSeekable(true)
		, mMono(false)
		, mDuration(0)
	{
	}

	void OggFileBuffer::open(const string& fileName)
	{
		//mOggCallbacks.read_func = oggRead;
		//mOggCallbacks.close_func = oggClose;
		//mOggCallbacks.seek_func = oggSeek;
		//mOggCallbacks.tell_func = oggTell;

		mFileName = fileName;


		//FILE* file = fopen(fileName.c_str(), "rb");
		//int result = ov_open_callbacks(file, &mOggFile, NULL, 0, mOggCallbacks);

		int result = ov_fopen(fileName.c_str(), &mOggFile);

		if(result < 0)
			return;

		mVorbisInfo = ov_info(&mOggFile, -1);
		mVorbisComment = ov_comment(&mOggFile, -1);

		mDuration = static_cast<float>(ov_time_total(&mOggFile, -1));
		mSeekable = (ov_seekable(&mOggFile) != 0);

		if (!getBufferInfo())
			return;

		mMono = ((mFormat==AL_FORMAT_MONO16) || (mFormat==AL_FORMAT_MONO8));
	}

	void OggFileBuffer::close()
	{
		ov_clear(&mOggFile);
	}

	void OggFileBuffer::reopen()
	{
		close();
		open(mFileName);
	}

	void OggFileBuffer::seekTime(float time)
	{
		ov_time_seek(&mOggFile, time);
	}

	void OggFileBuffer::seekBytes(long pos)
	{
		ov_raw_seek(&mOggFile, pos);
	}

	void OggFileBuffer::fill(ALuint buffer)
	{
		std::vector<char> data;
		char* chunk = new/*sound*/char[mChunkSize];

		int sizeRead = 1;
		int lastPos = 0;
		int section;

		while(sizeRead > 0)
		{
			//std::cerr << "reading : " << sizeRead << " at pos : " << lastPos << std::endl;
			sizeRead = ov_read(&mOggFile, chunk, mChunkSize, 0, 2, 1, &section);
			data.insert(data.end(), chunk, chunk + sizeRead);
			lastPos += sizeRead;
		}

		alGetError();
		alBufferData(buffer, mFormat, &data[0], lastPos, mVorbisInfo->rate);

		delete chunk;
	}

	bool OggFileBuffer::fillChunk(ALuint buffer)
	{
		char* data = new/*sound*/char[mChunkSize];
		
		bool lastChunk = readChunk(data, mChunkSize);

		alBufferData(buffer, mFormat, data, mChunkSize, mVorbisInfo->rate);
		alGetError();

		delete data;
		return lastChunk;
	}

	bool OggFileBuffer::readChunk(char* dataChunk, size_t chunkSize)
	{
		size_t size = 0;
		int  section;
		int result;
		bool lastChunk;

		while(size < mChunkSize)
		{
			result = ov_read(&mOggFile, dataChunk + size, mChunkSize - size, 0, 2, 1, &section);

			if(result > 0)
				size += result;
			else if(result == 0)
				lastChunk = false;
			else if(result < 0)
				break;
		}

		return lastChunk;
	}


	bool OggFileBuffer::getBufferInfo()
	{
		/** Calculates buffer size and format.
		@remarks
			Calculates a block aligned buffer size of 250ms using
			sound properties.
		 */
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...

		switch(mVorbisInfo->channels)
		{
		case 1:
				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate >> 1;
				mChunkSize -= (mChunkSize % 2);
			break;
		case 2:
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate;
				mChunkSize -= (mChunkSize % 4);
			break;
		case 4:
				mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
				if (!mFormat) return false;
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate * 2;
				mChunkSize -= (mChunkSize % 8);
			break;
		case 6:
				mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
				if (!mFormat) return false;
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate * 3;
				mChunkSize -= (mChunkSize % 12);
			break;
		case 7:
				mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
				if (!mFormat) return false;
				// Set BufferSize to 250ms (Frequency * 16 (16bit 7-channel) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate * 4;
				mChunkSize -= (mChunkSize % 16);
			break;
		case 8:
				mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
				if (!mFormat) return false;
				// Set BufferSize to 250ms (Frequency * 20 (16bit 8-channel) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate * 5;
				mChunkSize -= (mChunkSize % 20);
			break;
		default:
				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mChunkSize = mVorbisInfo->rate >> 1;
				mChunkSize -= (mChunkSize % 2);
			break;
		}
		return true;
	}
}
