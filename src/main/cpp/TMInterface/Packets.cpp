#include "TMInterface/Packets.h"

namespace TMInterface {

Packet::Packet(int32_t packetId, const std::string& packetName) : packetId(packetId), packetName(packetName) {}

Packet::~Packet() {}

int32_t Packet::registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> constructor) {
	registeredPackets.insert(std::make_pair(id, constructor));

	return id;
}

std::unique_ptr<Packet> Packet::getPacketById(int32_t id) {
	return registeredPackets.at(id)();
}

namespace Packets {

void S_RESPONSE::write(Interface& interface) const {
	interface.writeObj(test);
	interface.writeObj(test2);
}

void S_RESPONSE::read(Interface& interface) {
	interface.readObj(test);
	interface.readObj(test2);
}

}  // namespace Packets

}  // namespace TMInterface
