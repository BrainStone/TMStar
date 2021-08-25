#pragma once

#include <string>

template<size_t BUF_SIZE>
class NamedBuffer {
public:
	char* buffer;

private:
	void* hMapFile;

public:
	NamedBuffer(const std::string& bufferName);
	virtual ~NamedBuffer();

	bool isOk();
	operator bool();

	// Delete copy stuff
	NamedBuffer(const NamedBuffer&) = delete;
	NamedBuffer& operator= (const NamedBuffer&) = delete;
};

#define NamedBuffer_Proper_Included

#include "NamedBuffer.inc.cpp"

#undef NamedBuffer_Proper_Included
