#pragma once

#include "Exceptions/MapExceptions.h"

#include <unordered_map>

namespace CatanUtils {

	namespace MapUtils {

		template <typename T, typename S>
		std::unordered_map<T, S> get_combine_maps(const std::unordered_map<T, S>& first, const std::unordered_map<T, S>& second) {
			std::unordered_map<T, S> result = first;
			for (const auto& key : second) {
				if (result.find(key.first) == result.end()) {
					result.try_emplace(key.first, key.second);
				}
				else {
					result.at(key.first) += key.second;
				}
			}
			return result;
		}

		template <typename T, typename S>
		std::unordered_map<T, S> get_slashe_maps(const std::unordered_map<T, S>& first, const std::unordered_map<T, S>& second) {
			std::unordered_map<T, S> result = first;
			for (const auto& key : second) {
				if (result.find(key.first) == result.end() || result.at(key.first) < second.at(key.first)) {
					throw SlashFromEmptyValue("you are tring to slash from empty value");
				}
				else {
					result.at(key.first) -= key.second;
				}
			}
			return result;
		}

		template <typename T>
		uint32_t get_sum_of_values(const std::unordered_map<T, uint32_t>& map_to_sum) {

			uint32_t result = 0;
			for (const auto& element : map_to_sum) {
				result += element.second;
			}
			return result;
		}
	}
}
