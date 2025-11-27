#pragma once
#include <vector>
#include <memory>
#include "Token.hpp"
#include "CodeBuilder.hpp"
#include "../Engine/Runnable.hpp"

namespace Code::Fusion
{
    class FusionCodeGenerator
    {
    public:
        explicit FusionCodeGenerator(std::vector<std::unique_ptr<Token>> tokens) : m_tokens(std::move(tokens)), m_it(m_tokens.begin()) {}

        void generate();

        Token const *getCurrent() const;

        template <class T>
        T const *getTokenOrError(std::string const &errorMessage)
        {
            if (T const *t = dynamic_cast<T const *>(getCurrent()); t != nullptr)
            {
                return t;
            }
            error(errorMessage);
            return nullptr;
        }

        template <class T>
        bool isOfType()
        {
            return dynamic_cast<T const *>(getCurrent()) != nullptr;
        }

        bool isKeyword(Keyword keyword) const;

        bool isSeparator(Separator separator) const;

        void parseTypeDeclaration();

        void parseFunctionDeclaration();

        void consumeKeyword(Keyword keyword, std::string const &errorMessage);

        void consumeSeparator(Separator separator, std::string const &errorMessage);

        void consumeIdSeparator(std::string const &id, std::string const &errorMessage);

        Engine::Runnable::CodeConstantValue parseConstant(std::string const &errorMessage);

        /// @brief Try to consume separator and if the current value is not separator of needed value, do nothing. useful for skipping line breaks
        /// @param separator
        /// @param errorMessage
        void optionallyConsumeSeparator(Separator separator);

        void advance() { m_it++; }

        CodeBuilder const &getBuilder() const { return m_builder; }

    private:
        void error(std::string const &errorMessage);
        CodeBuilder m_builder;
        std::vector<std::unique_ptr<Token>> m_tokens;

        std::vector<std::unique_ptr<Token>>::const_iterator m_it;
    };

}