#include <unordered_map>
#include "Game/Board/Resources/Resource.h"

template <typename T, typename S>
std::unordered_map<T, S> get_combine_maps(const std::unordered_map<T, S>& first, const std::unordered_map<T, S>& second) {
	std::unordered_map<T, S> result = first;
	for (const auto& key : second){
		if (result.find(key.first) == result.end()) {
			result.try_emplace(key.first, key.second);
		}
		else {
			result.at(key.first) += key.second;
		}
	}
	return result;
}