#pragma once

#include <functional>
#include <map>

#include "Interface.h"

namespace TMInterface {

class Interface;

class Packet {
public:
	const int32_t packetId;

protected:
	static int32_t packetCount;
	static std::map<int32_t, std::reference_wrapper<Packet>> registeredPackets;

public:
	virtual void write(Interface& interface) const = 0;
	virtual void read(const Interface& interface) = 0;

	Packet();
	virtual ~Packet();

	static Packet& getPacketById(int32_t id);
};

}  // namespace TMInterface
