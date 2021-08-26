#pragma once

#include <atomic>
#include <string>

#include "Utils/NamedBuffer.h"
#include "Constants.h"

namespace TMInterface {

class Interface {
protected:
	Utils::NamedBuffer buffer;
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

}  // namespace TMInterface
