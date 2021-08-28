#include "TMInterface/Packets.h"

namespace TMInterface {

std::map<int32_t, std::vector<Packet::callback_t>> Packet::callbacks;

// We want to register a few default callbacks
std::unique_ptr<Packets::C_PROCESSED_CALL> autoResponse(std::shared_ptr<Packet> packet,
                                                        std::unique_ptr<Packets::C_PROCESSED_CALL> response) {
	response->which = packet->packetId;

	return response;
}

int __callbacksInit = []() {
	Packets::S_ON_REGISTERED::registerCallback(autoResponse);

	return 0;
}();

Packet::Packet(int32_t packetId, const std::string& packetName, int32_t responsePacketId)
    : packetId(packetId), packetName(packetName), responsePacketId(responsePacketId) {}

Packet::~Packet() {}

void Packet::registerCallback(const callback_t& callback) {
	callbacks[packetId].push_back(callback);
}

Packet::callback_t::result_type Packet::callCallbacks(callback_t::result_type packet) {
	for (const callback_t& callback : callbacks[packetId]) {
		packet = callback(std::shared_ptr<Packet>{this, [](Packet*) {}}, std::move(packet));
	}

	return packet;
}

int32_t Packet::registerPacket(int32_t id, std::function<std::unique_ptr<Packet>()> constructor) {
	registeredPackets.insert(std::make_pair(id, constructor));

	return id;
}

std::unique_ptr<Packet> Packet::getPacketById(int32_t id) {
	decltype(registeredPackets)::const_iterator it = registeredPackets.find(id);

	return (it == registeredPackets.end()) ? nullptr : it->second();
}

EmptyPacket::EmptyPacket(int32_t packetId, const std::string& packetName, int32_t responsePacketId)
    : Packet(packetId, packetName, responsePacketId) {}

EmptyPacket::~EmptyPacket() {}

void EmptyPacket::write(Interface& interface) const {
	// technically not needed. Just to be sure
	interface.writeObj(reserved);
}

void EmptyPacket::read(Interface& interface) {
	// technically not needed. Just to be sure
	interface.readObj(reserved);
}

namespace Packets {

void S_RESPONSE::write(Interface& interface) const {
	interface.writeObj(test);
	interface.writeObj(test2);
}
void S_RESPONSE::read(Interface& interface) {
	interface.readObj(test);
	interface.readObj(test2);
}

void C_PROCESSED_CALL::write(Interface& interface) const {
	interface.writeObj(which);
}
void C_PROCESSED_CALL::read(Interface& interface) {
	interface.readObj(which);
}

// Define registerCallback's
#define CreateCallbackDefinition(name)                                                                              \
	void name::registerCallback(const specializedCallback_t& callback) {                                            \
		name{}.Packet::registerCallback([callback](std::shared_ptr<Packet> packet,                                  \
		                                           std::unique_ptr<Packet> response) -> std::unique_ptr<Packet> {   \
			return callback(std::static_pointer_cast<name>(packet),                                                 \
			                std::unique_ptr<responsePacket_t>{static_cast<responsePacket_t*>(response.release())}); \
		});                                                                                                         \
	}

CreateCallbackDefinition(S_RESPONSE);
CreateCallbackDefinition(S_ON_REGISTERED);
CreateCallbackDefinition(S_SHUTDOWN);
CreateCallbackDefinition(S_ON_RUN_STEP);
CreateCallbackDefinition(S_ON_SIM_BEGIN);
CreateCallbackDefinition(S_ON_SIM_STEP);
CreateCallbackDefinition(S_ON_SIM_END);
CreateCallbackDefinition(S_ON_CHECKPOINT_COUNT_CHANGED);
CreateCallbackDefinition(S_ON_LAPS_COUNT_CHANGED);
CreateCallbackDefinition(S_ON_CUSTOM_COMMAND);
CreateCallbackDefinition(S_ON_BRUTEFORCE_EVALUATE);
CreateCallbackDefinition(C_REGISTER);
CreateCallbackDefinition(C_DEREGISTER);
CreateCallbackDefinition(C_PROCESSED_CALL);
CreateCallbackDefinition(C_SET_INPUT_STATES);
CreateCallbackDefinition(C_RESPAWN);
CreateCallbackDefinition(C_SIM_REWIND_TO_STATE);
CreateCallbackDefinition(C_SIM_GET_STATE);
CreateCallbackDefinition(C_SIM_GET_EVENT_BUFFER);
CreateCallbackDefinition(C_GET_CONTEXT_MODE);
CreateCallbackDefinition(C_SIM_SET_EVENT_BUFFER);
CreateCallbackDefinition(C_GET_CHECKPOINT_STATE);
CreateCallbackDefinition(C_SET_CHECKPOINT_STATE);
CreateCallbackDefinition(C_SET_GAME_SPEED);
CreateCallbackDefinition(C_EXECUTE_COMMAND);
CreateCallbackDefinition(C_SET_EXECUTE_COMMANDS);
CreateCallbackDefinition(C_SET_TIMEOUT);
CreateCallbackDefinition(C_REMOVE_STATE_VALIDATION);
CreateCallbackDefinition(C_PREVENT_SIMULATION_FINISH);
CreateCallbackDefinition(C_REGISTER_CUSTOM_COMMAND);
CreateCallbackDefinition(C_LOG);
CreateCallbackDefinition(ANY);

// Be gone
#undef CreateCallbackDefinition

}  // namespace Packets

}  // namespace TMInterface
