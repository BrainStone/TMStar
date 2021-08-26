#include "NamedBuffer.h"

#include <algorithm>
#include <iostream>

#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

#undef max
#undef min

NamedBuffer::NamedBuffer(const std::string& bufferName, size_t buf_size) : buf_size(buf_size), buffer(nullptr), hMapFile(nullptr) {
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		bufferName.c_str());   // name of mapping object

	if (hMapFile == nullptr) {
		std::cerr << "Could not open file mapping object (" << GetLastError() << ")." << std::endl;
		return;
	}

	buffer = reinterpret_cast<char*>(
		MapViewOfFile(hMapFile,  // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			buf_size)
		);

	if (buffer == nullptr) {
		std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
		return;

		CloseHandle(hMapFile);

		return;
	}

	zero();
}

NamedBuffer::~NamedBuffer() {
	if (buffer != nullptr) {
		zero();

		UnmapViewOfFile(buffer);
	}

	if (hMapFile != nullptr) {
		CloseHandle(hMapFile);
	}
}

bool NamedBuffer::isOk() const {
	return buffer != nullptr;
}

NamedBuffer::operator bool() const {
	return isOk();
}

void NamedBuffer::zero() {
	std::fill_n(buffer, buf_size, 0);
}
