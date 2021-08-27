#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "Interface.h"

namespace TMInterface {

// Forward declaration
class Interface;

class Packet {
public:
	// Parameters:
	//   - received packet
	//   - response packet (may be null)
	// Returns: response packet (may be null)
	using callback_t = std::function<std::unique_ptr<Packet>(std::shared_ptr<Packet>, std::unique_ptr<Packet>)>;

	const int32_t packetId;
	const std::string packetName;
	const int32_t responsePacketId;

protected:
	static inline std::map<int32_t, std::function<std::unique_ptr<Packet>()>> registeredPackets{};
	static std::map<int32_t, std::vector<callback_t>> callbacks;

public:
	Packet(int32_t packetId, const std::string& packetName, int32_t responsePacketId);
	virtual ~Packet();

	virtual void write(Interface& interface) const = 0;
	virtual void read(Interface& interface) = 0;

	virtual void registerCallback(const callback_t& callback);
	virtual callback_t::result_type callCallbacks(callback_t::result_type packet);

	static int32_t registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> constructor);
	static std::unique_ptr<Packet> getPacketById(int32_t id);
};

namespace Packets {
constexpr int32_t startCount = __COUNTER__ + 1;

constexpr int32_t NONE = 0;

// Unholy automatic packet declaration
#define Declare_Packet(name, responsePacketId, members)                                        \
	struct name : public Packet {                                                              \
		inline name() : Packet((__COUNTER__ - startCount) / 2 + 1, #name, responsePacketId) {} \
		inline virtual ~name() {}                                                              \
		virtual void write(Interface& interface) const;                                        \
		virtual void read(Interface& interface);                                               \
                                                                                               \
		members                                                                                \
	};                                                                                         \
	inline const int32_t name##_ID =                                                           \
	    Packet::registerPacket((__COUNTER__ - startCount) / 2 + 1, [] { return std::make_unique<name>(); });

Declare_Packet(S_RESPONSE, NONE, int32_t test = 0; int32_t test2 = 0;);

Declare_Packet(S_ON_REGISTERED, NONE, /**/);

#undef Declare_Packet
}  // namespace Packets

}  // namespace TMInterface
