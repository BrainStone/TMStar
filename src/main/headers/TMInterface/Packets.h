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

struct EmptyPacket : public Packet {
	int32_t reserved = 0;

	EmptyPacket(int32_t packetId, const std::string& packetName, int32_t responsePacketId);
	virtual ~EmptyPacket();

	virtual void write(Interface& interface) const;
	virtual void read(Interface& interface);
};

namespace Packets {
// Helper variable, to ensure the __COUNTER__ magic works
constexpr int32_t startCount = __COUNTER__ + 1;

// Little helper declarations for packets that don't have responses
using NONE = Packet;
constexpr int32_t NONE_ID = 0;

// Unholy automatic packet declaration macros
#define ForwardDeclarePacket(name) \
	class name;                    \
	extern const int32_t name##_ID;

#define CreateCallback(name, responsePacket)                                                                          \
	using specializedCallback_t =                                                                                     \
	    std::function<std::unique_ptr<responsePacket>(std::shared_ptr<name>, std::unique_ptr<responsePacket>)>;       \
                                                                                                                      \
	static inline void registerCallback(const specializedCallback_t& callback) {                                      \
		name{}.Packet::registerCallback(                                                                              \
		    [callback](std::shared_ptr<Packet> packet, std::unique_ptr<Packet> response) -> std::unique_ptr<Packet> { \
			    return callback(std::static_pointer_cast<name>(packet),                                               \
			                    std::unique_ptr<responsePacket>{static_cast<responsePacket*>(response.release()),     \
			                                                    response.get_deleter()});                             \
		    });                                                                                                       \
	}
#define RegisterPacket(name)         \
	inline const int32_t name##_ID = \
	    Packet::registerPacket((__COUNTER__ - startCount) / 2 + 1, [] { return std::make_unique<name>(); });

#define DeclareEmptyPacket(name, responsePacket)                                                       \
	struct name : public EmptyPacket {                                                                 \
		inline name() : EmptyPacket((__COUNTER__ - startCount) / 2 + 1, #name, responsePacket##_ID) {} \
		inline virtual ~name() {}                                                                      \
                                                                                                       \
		CreateCallback(name, responsePacket);                                                          \
	};                                                                                                 \
	RegisterPacket(name);
#define DeclarePacket(name, responsePacket, members)                                              \
	struct name : public Packet {                                                                 \
		inline name() : Packet((__COUNTER__ - startCount) / 2 + 1, #name, responsePacket##_ID) {} \
		inline virtual ~name() {}                                                                 \
		virtual void write(Interface& interface) const;                                           \
		virtual void read(Interface& interface);                                                  \
                                                                                                  \
		CreateCallback(name, responsePacket);                                                     \
                                                                                                  \
		members;                                                                                  \
	};                                                                                            \
	RegisterPacket(name);

// Forward declarations
ForwardDeclarePacket(S_RESPONSE);
ForwardDeclarePacket(S_ON_REGISTERED);

// Actual declarations
DeclarePacket(S_RESPONSE, NONE, int32_t test = 0; int32_t test2 = 0;);

DeclareEmptyPacket(S_ON_REGISTERED, NONE);

#undef ForwardDeclarePacket

#undef CreateCallback
#undef RegisterPacket

#undef DeclareEmptyPacket
#undef DeclarePacket

}  // namespace Packets

}  // namespace TMInterface
