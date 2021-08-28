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

#define CreateCallback(name, responsePacket)                                                                        \
	using responsePacket_t = responsePacket;                                                                        \
	using specializedCallback_t =                                                                                   \
	    std::function<std::unique_ptr<responsePacket_t>(std::shared_ptr<name>, std::unique_ptr<responsePacket_t>)>; \
                                                                                                                    \
	static void registerCallback(const specializedCallback_t& callback);
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
ForwardDeclarePacket(S_SHUTDOWN);
ForwardDeclarePacket(S_ON_RUN_STEP);
ForwardDeclarePacket(S_ON_SIM_BEGIN);
ForwardDeclarePacket(S_ON_SIM_STEP);
ForwardDeclarePacket(S_ON_SIM_END);
ForwardDeclarePacket(S_ON_CHECKPOINT_COUNT_CHANGED);
ForwardDeclarePacket(S_ON_LAPS_COUNT_CHANGED);
ForwardDeclarePacket(S_ON_CUSTOM_COMMAND);
ForwardDeclarePacket(S_ON_BRUTEFORCE_EVALUATE);
ForwardDeclarePacket(C_REGISTER);
ForwardDeclarePacket(C_DEREGISTER);
ForwardDeclarePacket(C_PROCESSED_CALL);
ForwardDeclarePacket(C_SET_INPUT_STATES);
ForwardDeclarePacket(C_RESPAWN);
ForwardDeclarePacket(C_SIM_REWIND_TO_STATE);
ForwardDeclarePacket(C_SIM_GET_STATE);
ForwardDeclarePacket(C_SIM_GET_EVENT_BUFFER);
ForwardDeclarePacket(C_GET_CONTEXT_MODE);
ForwardDeclarePacket(C_SIM_SET_EVENT_BUFFER);
ForwardDeclarePacket(C_GET_CHECKPOINT_STATE);
ForwardDeclarePacket(C_SET_CHECKPOINT_STATE);
ForwardDeclarePacket(C_SET_GAME_SPEED);
ForwardDeclarePacket(C_EXECUTE_COMMAND);
ForwardDeclarePacket(C_SET_EXECUTE_COMMANDS);
ForwardDeclarePacket(C_SET_TIMEOUT);
ForwardDeclarePacket(C_REMOVE_STATE_VALIDATION);
ForwardDeclarePacket(C_PREVENT_SIMULATION_FINISH);
ForwardDeclarePacket(C_REGISTER_CUSTOM_COMMAND);
ForwardDeclarePacket(C_LOG);
ForwardDeclarePacket(ANY);

// Actual declarations
DeclarePacket(S_RESPONSE, NONE, int32_t test = 0; int32_t test2 = 0;);

DeclareEmptyPacket(S_ON_REGISTERED, C_PROCESSED_CALL);
DeclareEmptyPacket(S_SHUTDOWN, NONE);
DeclareEmptyPacket(S_ON_RUN_STEP, NONE);
DeclareEmptyPacket(S_ON_SIM_BEGIN, NONE);
DeclareEmptyPacket(S_ON_SIM_STEP, NONE);
DeclareEmptyPacket(S_ON_SIM_END, NONE);
DeclareEmptyPacket(S_ON_CHECKPOINT_COUNT_CHANGED, NONE);
DeclareEmptyPacket(S_ON_LAPS_COUNT_CHANGED, NONE);
DeclareEmptyPacket(S_ON_CUSTOM_COMMAND, NONE);
DeclareEmptyPacket(S_ON_BRUTEFORCE_EVALUATE, NONE);
DeclareEmptyPacket(C_REGISTER, S_ON_REGISTERED);
DeclareEmptyPacket(C_DEREGISTER, NONE);

DeclarePacket(C_PROCESSED_CALL, NONE, int32_t which = ANY_ID;);

DeclareEmptyPacket(C_SET_INPUT_STATES, NONE);
DeclareEmptyPacket(C_RESPAWN, NONE);
DeclareEmptyPacket(C_SIM_REWIND_TO_STATE, NONE);
DeclareEmptyPacket(C_SIM_GET_STATE, NONE);
DeclareEmptyPacket(C_SIM_GET_EVENT_BUFFER, NONE);
DeclareEmptyPacket(C_GET_CONTEXT_MODE, NONE);
DeclareEmptyPacket(C_SIM_SET_EVENT_BUFFER, NONE);
DeclareEmptyPacket(C_GET_CHECKPOINT_STATE, NONE);
DeclareEmptyPacket(C_SET_CHECKPOINT_STATE, NONE);
DeclareEmptyPacket(C_SET_GAME_SPEED, NONE);
DeclareEmptyPacket(C_EXECUTE_COMMAND, NONE);
DeclareEmptyPacket(C_SET_EXECUTE_COMMANDS, NONE);
DeclareEmptyPacket(C_SET_TIMEOUT, NONE);
DeclareEmptyPacket(C_REMOVE_STATE_VALIDATION, NONE);
DeclareEmptyPacket(C_PREVENT_SIMULATION_FINISH, NONE);
DeclareEmptyPacket(C_REGISTER_CUSTOM_COMMAND, NONE);
DeclareEmptyPacket(C_LOG, NONE);
DeclareEmptyPacket(ANY, NONE);

// Remove evil marcos
#undef ForwardDeclarePacket

#undef CreateCallback
#undef RegisterPacket

#undef DeclareEmptyPacket
#undef DeclarePacket

}  // namespace Packets

}  // namespace TMInterface
