#include "TMInterface/Packets.h"

namespace TMInterface {

std::map<int32_t, std::reference_wrapper<Packet>> Packet::registeredPackets{};

Packet::Packet() {}

Packet& Packet::getPacketById(int32_t id) {
	return registeredPackets.at(id);
}

}  // namespace TMInterface
