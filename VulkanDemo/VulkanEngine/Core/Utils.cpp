#include "Utils.h"

namespace Rendering
{

	bool Utils::checkSupportOption(std::vector<char const*> const& availableOptions, std::vector<char const*> const& requestedOptions)
    {
        for (const auto& requestedOption : requestedOptions)
        {
            auto it = std::find_if(availableOptions.begin(), availableOptions.end(), [requestedOption](char const* availableOption)
                {
                    return std::strcmp(availableOption, requestedOption) == 0;
                });

            if (it == availableOptions.end())
                return false;
        }

        return true;
    }
}