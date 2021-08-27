#include "app.h"

#include <iostream>

#include "TMInterface/Interface.h"

int main() {
	std::cout << "Active interfaces:\n";

	for (std::shared_ptr<TMInterface::Interface> i : TMInterface::Interface::getActiveInterfaces()) {
		std::cout << i->getName() << '\n';
	}

	std::unique_ptr<TMInterface::Packet> packet = TMInterface::Packet::getPacketById(1);
	packet->registerCallback([](std::shared_ptr<TMInterface::Packet> packet, std::unique_ptr<TMInterface::Packet> response) {
		    std::cout << response->packetName << '\n';
		    return response;
	    });

	packet->callCallbacks(TMInterface::Packet::getPacketById(2));

	return 0;
}
