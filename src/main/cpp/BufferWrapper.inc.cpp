#pragma once

#include "BufferWrapper.h"

#ifdef BufferWrapper_Proper_Included

#include <algorithm>

template<size_t BUF_SIZE>
BufferWrapper<BUF_SIZE>::BufferWrapper(char* buffer) : buffer(buffer)
{
}

template<size_t BUF_SIZE>
void BufferWrapper<BUF_SIZE>::zero(size_t amount)
{
	std::fill(buffer, buffer + amount, 0);
}

template<size_t BUF_SIZE>
template<typename T>
void BufferWrapper<BUF_SIZE>::writeObj(const T& obj)
{
	const char* pointer = reinterpret_cast<const char*>(&obj);
	constexpr size_t size = sizeof(T);

	zero(size + 1);
	std::copy(pointer, pointer + size, buffer);
}

template<size_t BUF_SIZE>
template<typename T>
void BufferWrapper<BUF_SIZE>::readObj(T& obj) const
{
	char* pointer = reinterpret_cast<char*>(&obj);
	constexpr size_t size = sizeof(T);

	std::copy(buffer, buffer + size, pointer);
	zero(size + 1);
}

#endif