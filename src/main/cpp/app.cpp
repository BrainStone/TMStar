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

	buffer.buffer.writeObj(Message<EmptyData>{ MessageType::C_REGISTER, 0});
	buffer.buffer.buffer[1] = 0xFF;

	Sleep(1'000);

	Message<ExecuteCommandData> message{ MessageType::C_EXECUTE_COMMAND, 0, ExecuteCommandData{std::string{"help"}} };
	buffer.buffer.writeObj(message);
	buffer.buffer.buffer[1] = 0xFF;

	Sleep(1'000);

	Message<char[4086]> answer;
	buffer.buffer.readObj(answer);

	std::cout << answer.type << std::endl << answer.errorCode << std::endl << answer.data + 4 << std::endl;

	Sleep(1'000);

	buffer.buffer.writeObj(Message<EmptyData>{ MessageType::C_DEREGISTER, 0});
	buffer.buffer.buffer[1] = 0xFF;

	return 0;
}
