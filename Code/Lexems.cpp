#include "Lexems.hpp"

#include <algorithm>
std::string Code::getFusionInstructionText(FusionInstruction instruction)
{
    if (std::unordered_map<std::string, FusionInstruction>::const_iterator it = std::find_if(FusionInstructions.begin(), FusionInstructions.end(),
                                                                                             [instruction](auto const &v)
                                                                                             { return v.second == instruction; });
        it != FusionInstructions.end())
    {
        return it->first;
    }
    return "UKNOWN INSTRUCTION";
}