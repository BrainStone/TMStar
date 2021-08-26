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
	NamedBuffer(const std::string& bufferName, size_t buf_size, bool printErrors = true);
	virtual ~NamedBuffer();

	constexpr bool isOk() const;
	constexpr operator bool() const;

	// Delete copy stuff
	NamedBuffer(const NamedBuffer&) = delete;
	NamedBuffer& operator=(const NamedBuffer&) = delete;

protected:
	void zero();
};

// constexpr functions
constexpr bool NamedBuffer::isOk() const {
	return buffer != nullptr;
}

constexpr NamedBuffer::operator bool() const {
	return isOk();
}

}  // namespace Utils
}  // namespace TMInterface
