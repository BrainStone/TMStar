#include "TMInterface/Packets.h"

namespace TMInterface {

std::map<int32_t, std::vector<Packet::callback_t>> Packet::callbacks;

Packet::Packet(int32_t packetId, const std::string& packetName, int32_t responsePacketId)
    : packetId(packetId), packetName(packetName), responsePacketId(responsePacketId) {}

Packet::~Packet() {}

void Packet::registerCallback(const callback_t& callback) {
	callbacks[packetId].push_back(callback);
}

Packet::callback_t::result_type Packet::callCallbacks(callback_t::result_type packet) {
	for (const callback_t& callback : callbacks[packetId]) {
		packet = callback(std::shared_ptr<Packet>{this, [](Packet*) {}}, std::move(packet));
	}

	return packet;
}

int32_t Packet::registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> constructor) {
	registeredPackets.insert(std::make_pair(id, constructor));

	return id;
}

std::unique_ptr<Packet> Packet::getPacketById(int32_t id) {
	decltype(registeredPackets)::const_iterator it = registeredPackets.find(id);

	return (it == registeredPackets.end()) ? nullptr : it->second();
}

EmptyPacket::EmptyPacket(int32_t packetId, const std::string& packetName, int32_t responsePacketId)
    : Packet(packetId, packetName, responsePacketId) {}

EmptyPacket::~EmptyPacket() {}

void EmptyPacket::write(Interface& interface) const {
	// technically not needed. Just to be sure
	interface.writeObj(reserved);
}

void EmptyPacket::read(Interface& interface) {
	// technically not needed. Just to be sure
	interface.readObj(reserved);
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
