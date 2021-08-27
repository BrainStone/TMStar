#pragma once

#include <functional>
#include <map>
#include <memory>

#include "Interface.h"

namespace TMInterface {

// Forward declaration
class Interface;

class Packet {
public:
	const int32_t packetId;
	const std::string packetName;

protected:
	static inline std::map<int32_t, std::function<std::unique_ptr<Packet>()>> registeredPackets{};

public:
	Packet(int32_t packetId, const std::string& packetName);
	virtual ~Packet();

	virtual void write(Interface& interface) const = 0;
	virtual void read(Interface& interface) = 0;

	static int32_t registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> constructor);
	static std::unique_ptr<Packet> getPacketById(int32_t id);

	friend class Interface;
};

namespace Packets {
constexpr int32_t startCount = __COUNTER__ + 1;

// Unholy automatic packet declaration
#define Declare_Packet(name, members)                                        \
	struct name : public Packet {                                            \
		inline name() : Packet((__COUNTER__ - startCount) / 2 + 1, #name) {} \
		inline virtual ~name() {}                                            \
		virtual void write(Interface& interface) const;                      \
		virtual void read(Interface& interface);                             \
                                                                             \
		members                                                              \
	};                                                                       \
	inline const int32_t name##_ID =                                         \
	    Packet::registerPacket((__COUNTER__ - startCount) / 2 + 1, [] { return std::make_unique<name>(); });

Declare_Packet(S_RESPONSE, int32_t test = 0; int32_t test2 = 0;)

#undef Declare_Packet
}  // namespace Packets

}  // namespace TMInterface
