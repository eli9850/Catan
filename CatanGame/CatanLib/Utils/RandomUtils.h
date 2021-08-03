#pragma once

#include "Utils/TimeUtils.h"

#include <array>
#include <algorithm>
#include <random>

namespace CatanUtils
{
	namespace RandomUtils
	{
		template <typename T, uint32_t N>
		void shuffle_array(std::array<T, N>& array_to_shuffle)
		{
			const auto seed = CatanUtils::TimeUtils::get_time_since_epoch();
			shuffle(array_to_shuffle.begin(), array_to_shuffle.end(), std::default_random_engine(seed));
		}
	}
}
