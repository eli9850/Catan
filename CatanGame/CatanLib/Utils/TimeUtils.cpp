#include "TimeUtils.h"

#include <chrono>

namespace CatanUtils {

	namespace TimeUtils {

		uint32_t get_time_sicnce_epoch() {
			return static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
		}
	}
}