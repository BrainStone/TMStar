#pragma once

#include "Interface.h"

#ifdef TMInterface_Interface_Proper_Included

#include <algorithm>

namespace TMInterface {

// constexpr functions
constexpr bool Interface::isActive() const {
	return buffer.isOk();
}

constexpr Interface::operator bool() const {
	return isActive();
}

// template functions
template <typename T>
void Interface::writeObj(const T& obj) {
	const char* pointer = reinterpret_cast<const char*>(&obj);
	constexpr size_t size = sizeof(T);

	// TODO: Range check

	std::copy_n(pointer, size, buffer.buffer + bufferOffset);
	bufferOffset += size;
}

template <typename T>
void Interface::readObj(T& obj) {
	char* pointer = reinterpret_cast<char*>(&obj);
	constexpr size_t size = sizeof(T);

	// TODO: Range check

	std::copy_n(buffer.buffer + bufferOffset, size, pointer);
	bufferOffset += size;
}

}  // namespace TMInterface

#endif
