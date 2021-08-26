#include "TmInterface/Interface.h"

#include <sstream>

namespace TMInterface {

Interface::Interface(const std::string& name, bool printErrors)
    : name(name), buffer(name, BUF_SIZE, printErrors), registered(false) {}

Interface::Interface(size_t index, bool printErrors) : Interface(getNameFromIndex(index), printErrors) {}

Interface::~Interface() {}

const std::string& Interface::getName() const {
	return name;
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
