#pragma once

#include <map>
#include <functional>

namespace TMInterface {

class Packet {
protected:
	static std::map<int32_t, std::reference_wrapper<Packet>> registeredPackets;

public:
	virtual void write() const = 0;
	virtual void read() = 0;

	Packet();

	static Packet& getPacketById(int32_t id);
};

}  // namespace TMInterface
