#pragma once

#include <ostream>

namespace TMInterface {

constexpr size_t BUF_SIZE = 16384;
constexpr size_t MAX_SERVERS = 16;

enum class ErrorCode : int32_t {
	NONE = 0,
	RESPONSE_TOO_LONG,

	CLIENT_ALREADY_REGISTERED,

	NO_EVENT_BUFFER,

	NO_PLAYER_INFO
};

inline constexpr std::ostream& operator<<(std::ostream& os, const ErrorCode type) {
#define ENUMSTR(name)       \
	case ErrorCode::##name: \
		os << #name;        \
		break;
	switch (type) {
		ENUMSTR(NONE)
		ENUMSTR(RESPONSE_TOO_LONG)
		ENUMSTR(CLIENT_ALREADY_REGISTERED)
		ENUMSTR(NO_EVENT_BUFFER)
		ENUMSTR(NO_PLAYER_INFO)
	default:
		os << "Unknown error code: " << static_cast<int32_t>(type);
		break;
	}

#undef ENUMSTR
	return os;
}

}  // namespace TMInterface
