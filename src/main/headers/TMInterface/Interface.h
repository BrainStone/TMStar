#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "Constants.h"
#include "Packets.h"
#include "Utils/NamedBuffer.h"

namespace TMInterface {

// Forward declaration
class Packet;

namespace Packets {
struct S_RESPONSE;
}

class Interface {
protected:
	const std::string name;
	Utils::NamedBuffer buffer;
	std::atomic<size_t> bufferOffset;
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

	void sendPacket(const Packet& packet);
	std::unique_ptr<Packet> receivePacket();

	static std::vector<std::shared_ptr<Interface>> getActiveInterfaces();

protected:
	void zero(size_t amount = BUF_SIZE);

	template <typename T>
	void writeObj(const T& obj);
	template <typename T>
	void readObj(T& obj);

	static std::string getNameFromIndex(size_t index);

	// Packet needs access to protected member methods
	friend class Packet;
	friend class Packets::S_RESPONSE;
};

}  // namespace TMInterface

#define TMInterface_Interface_Proper_Included

#include "Interface.inc.h"

#undef TMInterface_Interface_Proper_Included
