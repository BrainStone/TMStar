#include "TmInterface/Interface.h"

#include <iostream>
#include <sstream>

namespace TMInterface {

Interface::Interface(const std::string& name, bool printErrors)
    : name(name), buffer(name, BUF_SIZE, printErrors), bufferOffset(0), registered(false) {}

Interface::Interface(size_t index, bool printErrors) : Interface(getNameFromIndex(index), printErrors) {}

Interface::~Interface() {}

const std::string& Interface::getName() const {
	return name;
}

void Interface::sendPacket(const Packet& packet) {
	std::cout << "Sending packet: " << packet.packetName << std::endl;

	zero();

	writeObj(packet.packetId);
	writeObj(ErrorCode::NONE);

	packet.write(*this);

	// Send packet
	buffer.buffer[1] = 0xFF;
}

std::unique_ptr<Packet> Interface::receivePacket() {
	bufferOffset = 0;

	if (buffer.buffer[1] != (char)0xFF) {
		// TODO throw error, packet not ready to receive!
		std::cout << "Error! packet not ready to receive" << std::endl;
	}

	buffer.buffer[1] = 0x00;

	int32_t packetId;
	ErrorCode error;

	readObj(packetId);
	readObj(error);

	if (error != ErrorCode::NONE) {
		// TODO throw error, error code received
		std::cout << "Error! " << error << std::endl;
	}

	std::unique_ptr<Packet> packet = Packet::getPacketById(packetId);

	std::cout << "Received packet: " << packetId << " -> " << packet->packetName << std::endl;

	packet->read(*this);

	// TODO: Only zero the package size
	zero();

	std::unique_ptr<Packet> response = packet->callCallbacks(Packet::getPacketById(packet->responsePacketId));

	if (response != nullptr) {
		sendPacket(*response);
	}

	return packet;
}

std::vector<std::shared_ptr<Interface>> Interface::getActiveInterfaces() {
	std::vector<std::shared_ptr<Interface>> list{};
	list.reserve(MAX_SERVERS);

	for (size_t i = 0; i < MAX_SERVERS; ++i) {
		std::shared_ptr<Interface> interface = std::make_shared<Interface>(i, false);

		if (*interface) list.push_back(interface);
	}

	list.shrink_to_fit();
	return list;
}

void Interface::zero(size_t amount) {
	// TODO implement check to prevent already zeroed buffer
	// Perrformance, not that important

	std::fill_n(buffer.buffer, amount, 0);
	bufferOffset = 0;
}

std::string Interface::getNameFromIndex(size_t index) {
	std::ostringstream stream;

	if (index >= MAX_SERVERS) {
		stream << "Index " << index << " out of range 0 to " << (MAX_SERVERS - 1);

		throw std::out_of_range(stream.str());
	}

	stream << "TMInterface" << index;

	return stream.str();
}

}  // namespace TMInterface
