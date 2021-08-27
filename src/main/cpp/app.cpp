#include "app.h"

#include <iostream>

#include "TMInterface/Interface.h"

int main() {
	std::cout << "Active interfaces:\n";

	for (std::shared_ptr<TMInterface::Interface> i : TMInterface::Interface::getActiveInterfaces()) {
		std::cout << i->getName() << '\n';
	}

	std::unique_ptr<TMInterface::Packet> packet = TMInterface::Packet::getPacketById(1);
	std::cout << packet->packetId << ": " << packet->packetName << std::endl;

	return 0;
}
