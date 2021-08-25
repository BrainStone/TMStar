#pragma once

#include "NamedBuffer.h"

#ifdef NamedBuffer_Proper_Included

#include <iostream>

#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

template<size_t BUF_SIZE>
NamedBuffer<BUF_SIZE>::NamedBuffer(const std::string& bufferName) : buffer(nullptr)
{
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		bufferName.c_str());   // name of mapping object

	if (hMapFile == nullptr) {
		std::cerr << "Could not open file mapping object (" << GetLastError() << ")." << std::endl;
		return;
	}

	buffer = reinterpret_cast<char*>(
		MapViewOfFile(hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			BUF_SIZE)
		);

	if (buffer == nullptr) {
		std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
		return;

		CloseHandle(hMapFile);

		return;
	}
}

template<size_t BUF_SIZE>
NamedBuffer<BUF_SIZE>::~NamedBuffer() {
	UnmapViewOfFile(buffer);

	CloseHandle(hMapFile);
}

template<size_t BUF_SIZE>
bool NamedBuffer<BUF_SIZE>::isOk() {
	return buffer != nullptr;
}

template<size_t BUF_SIZE>
NamedBuffer<BUF_SIZE>::operator bool() {
	return isOk();
}

#endif
