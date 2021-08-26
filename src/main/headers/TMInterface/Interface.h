#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "Constants.h"
#include "Utils/NamedBuffer.h"

namespace TMInterface {

class Interface {
protected:
	const std::string name;
	Utils::NamedBuffer buffer;
	std::atomic_bool registered;

public:
	Interface(const std::string& name, bool printErrors = true);
	Interface(size_t index = 0, bool printErrors = true);
	virtual ~Interface();

	// Delete copy stuff
	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;

	const std::string& getName() const;
	constexpr bool isActive() const;
	constexpr operator bool() const;

	static std::vector<std::shared_ptr<Interface>> getActiveInterfaces();

protected:
	static std::string getNameFromIndex(size_t index);
};

// constexpr functions
constexpr bool Interface::isActive() const {
	return buffer.isOk();
}

constexpr Interface::operator bool() const {
	return isActive();
}

}  // namespace TMInterface
