#pragma once

template <size_t BUF_SIZE>
class BufferWrapper {
public:
	char* buffer;

public:
	BufferWrapper(char* buffer);

	void zero(size_t amount = BUF_SIZE);

	template <typename T>
	void writeObj(const T& obj);
	template <typename T>
	void readObj(T& obj);
};

#define BufferWrapper_Proper_Included

#include "BufferWrapper.inc.cpp"

#undef BufferWrapper_Proper_Included
