#include <iostream>

#include "app.h"
#include "NamedBuffer.h"
#include "TMInterface.h"

int main() {
	std::string bufferName{ MAPPED_FILE_PREFIX "0" };
	NamedBuffer<BUF_SIZE> buffer{ bufferName };

	if (!buffer) {
		std::cerr << "Connection to " << bufferName << " failed!" << std::endl;
		return 1;
	}

	std::cout << "Successfully connected to " << bufferName << "!" << std::endl;

	Message<ExecuteCommandData> message{ MessageType::C_EXECUTE_COMMAND, 0, ExecuteCommandData{false, std::string{"help"}} };
	char* pointer = reinterpret_cast<char*>(&message);

	std::copy(pointer, pointer + sizeof(message), buffer.buffer);

	Sleep(1'000);

	Message<char[4086]> answer;
	pointer = reinterpret_cast<char*>(&answer);

	std::copy(buffer.buffer, buffer.buffer + sizeof(answer), pointer);

	std::cout << answer.type << std::endl << answer.errorCode << std::endl << answer.data + 4 << std::endl;

	return 0;
}
