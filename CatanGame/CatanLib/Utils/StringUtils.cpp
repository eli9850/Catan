#include "StringUtils.h"

namespace CatanUtils {

    std::vector<std::string> split(const std::string& data, const std::string& delimiter) {

        std::vector<std::string> result;
        size_t offset = 0;
        size_t position = 0;
        do
        {
            position = data.find(delimiter, offset);
            if (position == std::string::npos) {
                position = data.length();
            }
            std::string token = data.substr(offset, position - offset);
            if (!token.empty()) {
                result.push_back(token);
            }
            offset = position + delimiter.length();

        } while (position < data.length() && offset < data.length());

        return result;
    }
}