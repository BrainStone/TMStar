#include "app.h"

#include <windows.h>

#include <iostream>
#undef interface

#include "TMInterface/Interface.h"

int main() {
	std::cout << "Active interfaces:\n";

	for (std::shared_ptr<TMInterface::Interface> i : TMInterface::Interface::getActiveInterfaces()) {
		std::cout << i->getName() << '\n';
	}

	TMInterface::Packets::S_RESPONSE::registerCallback(
	    [](std::shared_ptr<TMInterface::Packets::S_RESPONSE> packet, std::unique_ptr<TMInterface::Packet> response) {
		    std::cout << packet->test << '\n';
		    std::cout << ((response == nullptr) ? "null" : response->packetName) << '\n';
		    return response;
	    });

	TMInterface::Interface interface{0};

	interface.sendPacket(TMInterface::Packets::C_REGISTER{});

	Sleep(1'000);

	std::cout << interface.receivePacket()->packetName << std::endl;

	Sleep(10'000);

	interface.sendPacket(TMInterface::Packets::C_DEREGISTER{});
	Sleep(1'000);

	std::cout << interface.receivePacket()->packetName << std::endl;

	return 0;
}
