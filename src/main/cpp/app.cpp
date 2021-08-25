#include <iostream>

#include "app.h"
#include "NamedBuffer.h"

int main() {
	std::string bufferName{ MAPPED_FILE_NAME "0" };
	NamedBuffer<BUF_SIZE> buffer{ bufferName };

	if (!buffer) std::cerr << "Connection to " << bufferName << " failed!" << std::endl;

	std::cout << "Successfully connected to " << bufferName << "!" << std::endl;

	return 0;
}
