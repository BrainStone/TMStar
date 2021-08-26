#pragma once

#include <atomic>
#include <string>

#include "NamedBuffer.h"

namespace TMInterface {

constexpr size_t BUF_SIZE = 16384;
constexpr size_t MAX_SERVERS = 16;

class Interface {
protected:
	NamedBuffer buffer;
	std::atomic_bool registered;

public:
	Interface(const std::string &name);
	Interface(size_t index = 0);
	virtual ~Interface();

	// Delete copy stuff
	Interface(const Interface &) = delete;
	Interface &operator=(const Interface &) = delete;

protected:
	static std::string getNameFromIndex(size_t index);
};

class Packet {};

}  // namespace TMInterface

#define TMInterface_Proper_Included

#include "TMInterface.inc.cpp"

#undef TMInterface_Proper_Included
