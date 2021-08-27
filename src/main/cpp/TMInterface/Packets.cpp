#include "TMInterface/Packets.h"

namespace TMInterface {

int32_t Packet::packetCount = 0;
std::map<int32_t, std::reference_wrapper<Packet>> Packet::registeredPackets{};

Packet::Packet() : packetId(++packetCount) {
	registeredPackets.insert(std::make_pair(packetId, std::ref(*this)));
}

Packet::~Packet() {}

Packet& Packet::getPacketById(int32_t id) {
	return registeredPackets.at(id);
}

}  // namespace TMInterface
