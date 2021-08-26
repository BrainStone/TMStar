#pragma once

namespace TMInterface {

class Packet {
protected:
public:
	virtual void write() const = 0;
	virtual void read() = 0;
};

}  // namespace TMInterface
