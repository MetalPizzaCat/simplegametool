#pragma once
#include <string>
#include <memory>
#include <vector>
#include <optional>

#include "Token.hpp"

namespace Code
{
    class Parser
    {
    public:
        explicit Parser(std::string const &code);

        std::vector<std::unique_ptr<Token>> parseTokens();

        bool isAtTheEnd() const { return m_current == m_end; }

        void advance();

        void advanceBy(size_t offset);

        void skipWhitespace();

        void skipComments();

        /// @brief Get `*(m_current + offset)`. If it goes out of bound return None
        /// @param offset
        /// @return
        std::optional<char> getFromCurrentWithOffset(size_t offset) const;

        bool checkString(std::string const &str);

        std::optional<SpecialCharacter> tryParseSpecialCharacter();

        std::optional<Separator> tryParseSeparator();

        std::optional<char> getCurrent();

        std::unique_ptr<KeywordToken> parseKeyword();

        std::unique_ptr<IdToken> parseId();

        std::unique_ptr<LabelToken> parseLabel();

        std::unique_ptr<StringToken> parseString();

        std::unique_ptr<AssetRefToken> parseAssetRef();

        std::unique_ptr<VariableToken> parseVariableName();

        std::unique_ptr<SeparatorToken> parseSeparator();

        std::unique_ptr<IntToken> parseInt();

        std::unique_ptr<FloatToken> parseFloat();

        std::unique_ptr<BoolToken> parseBool();

        std::unique_ptr<InstructionToken> parseInstruction();

    private:
        const std::string::const_iterator m_end;
        std::string::const_iterator m_current;
        size_t m_column;
        size_t m_row;
    };
}