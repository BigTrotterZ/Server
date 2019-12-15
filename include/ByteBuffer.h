#ifndef _BYTEBUFFER_H_
#define _BYTEBUFFER_H_

#include "Common.h"


class ByteBuffer
{
#define DEFAULT_SIZE 0x1000
#define DEFAULT_INCREASE_SIZE 200

private:
	uint8 * m_buffer;
	size_t m_readPos;
	size_t m_writePos;
	uint32 m_buffersize;
	
public:
	ByteBuffer()
	{
	    m_buffer =0;
	    m_readPos = m_writePos = 0;
	    m_buffersize = 0;
	    reserve(DEFAULT_SIZE);
	}

	ByteBuffer(size_t res)
	{
	    m_buffer =0;
	    m_readPos = m_writePos = 0;
	    m_buffersize = 0;
	        reserve(res);
	}
	~ByteBuffer()
	{
		free(m_buffer);
	}

	inline const uint8 * contents()
	{
		return m_buffer;
	}

	void reserve(size_t res)
	{
		if(m_buffer)
			m_buffer = (uint8*)realloc(m_buffer, res);
		else
			m_buffer = (uint8*)malloc(res);

		m_buffersize = res;
	}

	inline void EnsureBufferSize(uint32 Bytes)
	{
		size_t  new_size = m_writePos + Bytes;
		if(new_size > m_buffersize)
        {
                new_size = (new_size / DEFAULT_INCREASE_SIZE + 1) * DEFAULT_INCREASE_SIZE;
                reserve(new_size);
        }

	}

	template<typename T>
		T Read()
	{
		if(m_readPos + sizeof(T) > m_writePos)
			return (T)0;
		T ret = *(T*)&m_buffer[m_readPos];
		m_readPos += sizeof(T);
		return ret;
	}

#define DEFINE_FAST_READ_OPERATOR(type, size) ByteBuffer& operator >> (type& dest) { if(m_readPos + size > m_writePos) { dest = (type)0; return *this; } else { dest = *(type*)&m_buffer[m_readPos]; m_readPos += size; return *this; } }
#define DEFINE_FAST_WRITE_OPERATOR(type, size) ByteBuffer& operator << (const type src) { if(m_writePos + size > m_buffersize) { reserve(m_buffersize + DEFAULT_INCREASE_SIZE); } *(type*)&m_buffer[m_writePos] = src; m_writePos += size; return *this; }

	DEFINE_FAST_READ_OPERATOR(uint64, 8);
	DEFINE_FAST_READ_OPERATOR(uint32, 4);
	DEFINE_FAST_READ_OPERATOR(uint16, 2);
	DEFINE_FAST_READ_OPERATOR(uint8, 1);
	DEFINE_FAST_READ_OPERATOR(int64, 8);
	DEFINE_FAST_READ_OPERATOR(int32, 4);
	DEFINE_FAST_READ_OPERATOR(int16, 2);
	DEFINE_FAST_READ_OPERATOR(int8, 1);
	DEFINE_FAST_READ_OPERATOR(float, 4);
	DEFINE_FAST_READ_OPERATOR(double, 8);

	DEFINE_FAST_WRITE_OPERATOR(uint64, 8);
	DEFINE_FAST_WRITE_OPERATOR(uint32, 4);
	DEFINE_FAST_WRITE_OPERATOR(uint16, 2);
	DEFINE_FAST_WRITE_OPERATOR(uint8, 1);
	DEFINE_FAST_WRITE_OPERATOR(int64, 8);
	DEFINE_FAST_WRITE_OPERATOR(int32, 4);
	DEFINE_FAST_WRITE_OPERATOR(int16, 2);
	DEFINE_FAST_WRITE_OPERATOR(int8, 1);
	DEFINE_FAST_WRITE_OPERATOR(float, 4);
	DEFINE_FAST_WRITE_OPERATOR(double, 8);

	ByteBuffer & operator << (const std::string & value) 
	{
		EnsureBufferSize(value.length() + 1);
		memcpy(&m_buffer[m_writePos], value.c_str(), value.length()+1);
		m_writePos += (value.length() + 1);
		return *this;
	}

	ByteBuffer& operator >> (std::string & dest)
	{
		dest.clear();
		char c;
		for(;;)
		{
			c = Read<char>();
			if(c == 0) break;
			dest += c;
		}
		return *this;
	}

	inline void clear()
	{
		m_readPos = m_writePos = 0;
	}

	inline size_t size() { return m_writePos; }
};



#endif