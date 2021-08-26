#pragma once

#include <string>

namespace TMInterface {
namespace Utils {

class NamedBuffer {
public:
	size_t buf_size;
	char* buffer;

protected:
	void* hMapFile;

public:
	NamedBuffer(const std::string& bufferName, size_t buf_size);
	virtual ~NamedBuffer();

	bool isOk() const;
	operator bool() const;

	// Delete copy stuff
	NamedBuffer(const NamedBuffer&) = delete;
	NamedBuffer& operator=(const NamedBuffer&) = delete;

protected:
	void zero();
};

}  // namespace Utils
}  // namespace TMInterface