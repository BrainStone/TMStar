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

	buffer.buffer.writeObj(Message<EmptyData>{ MessageType::C_REGISTER });
	buffer.buffer.buffer[1] = 0xFF;

	Sleep(1'000);

	//buffer.buffer.buffer[1] = 0x00;
	Message<EmptyData> answer1;
	buffer.buffer.readObj(answer1);

	std::cout << answer1.type << std::endl << answer1.errorCode << std::endl;

	buffer.buffer.writeObj(Message<ProcessedCallData>{ MessageType::C_PROCESSED_CALL, ErrorCode::NONE, ProcessedCallData{ MessageType::S_ON_REGISTERED } });
	buffer.buffer.buffer[1] = 0xFF;

	Sleep(1'000);

	ExecuteCommandData data{ std::string{"help"} };
	std::cout << data.commandSize << std::endl << data.command << std::endl;

	Message<ExecuteCommandData> message{ MessageType::C_EXECUTE_COMMAND, ErrorCode::NONE, ExecuteCommandData{std::string{"help"}} };
	buffer.buffer.writeObj(message);
	buffer.buffer.buffer[1] = 0xFF;

	Sleep(1'000);

	//buffer.buffer.buffer[1] = 0x00;
	Message<char[4086]> answer2;
	buffer.buffer.readObj(answer2);

	std::cout << answer2.type << std::endl << answer2.errorCode << std::endl << answer2.data + 4 << std::endl;

	Sleep(1'000);

	buffer.buffer.writeObj(Message<EmptyData>{ MessageType::C_DEREGISTER });
	buffer.buffer.buffer[1] = 0xFF;

	return 0;
}
