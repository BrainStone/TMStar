#include "app.h"

#include <iostream>

#include "TMInterface/Interface.h"

int main() {
	std::cout << "Active interfaces:\n";

	for (std::shared_ptr<TMInterface::Interface> i : TMInterface::Interface::getActiveInterfaces()) {
		std::cout << i->getName() << '\n';
	}

	std::cout.flush();

	return 0;
}
