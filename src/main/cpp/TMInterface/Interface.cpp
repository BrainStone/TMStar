#include "TmInterface/Interface.h"

#include <sstream>

namespace TMInterface {

Interface::Interface(const std::string& name) : buffer(name, BUF_SIZE), registered(false) {}

Interface::Interface(size_t index) : Interface(getNameFromIndex(index)) {}

Interface::~Interface() {}

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
