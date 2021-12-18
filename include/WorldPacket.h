#ifndef _WORLD_PACKET_H_
#define _WORLD_PACKET_H_

#include "ByteBuffer.h"

class WorldPacket : public ByteBuffer
{
public:
	inline WorldPacket() : ByteBuffer(), m_opcode(0) { }
	inline WorldPacket(uint16 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) {}
	inline WorldPacket(size_t res) : ByteBuffer(res), m_opcode(0) { }
	inline WorldPacket(const WorldPacket &packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) {}

public:
	inline void Initialize(uint16 opcode )
    {
        clear();
        m_opcode = opcode;
    }

	inline uint16 GetOpcode() const { return m_opcode; }
    inline void SetOpcode(const uint16 & opcode) { m_opcode = opcode; }
	inline void SetLength(const uint16 & len)	
	{ 
		uint16 * plen = (uint16 * ) &(contents()[2]);
		*plen = len;
	}

	template<typename T> void SetOffset(const uint16 & offset, const T value ) 
	{
		T * pval = (T *) &(contents()[offset]);
		*pval = value;
	}
	
protected:
	uint16 m_opcode;
};

#endif