#pragma once
#include <vector>
#include <memory>
#include "Token.hpp"
#include "CodeBuilder.hpp"
#include "../Engine/Execution/Runnable.hpp"

namespace Code::Fusion
{

    class FusionCodeGenerator
    {
    public:
        explicit FusionCodeGenerator(std::vector<std::unique_ptr<Token>> tokens, std::vector<std::string> const &defaultTypes);

        void generate();

        Token const *getCurrent() const;

        /// @brief Attempt to get token of given type or throw an error if current token is null or of a wrong type without advancing the iterator
        /// @tparam T Type of the token
        /// @param errorMessage Message to display on failure
        /// @return Casted token
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

        /// @brief Check if current token is of a given type. Works as a wrapper around c++ casting for simplicity
        /// @tparam T Type of the token to check for
        /// @return False if of a wrong type or null, true otherwise
        template <class T>
        bool isOfType()
        {
            return dynamic_cast<T const *>(getCurrent()) != nullptr;
        }

        bool isKeyword(Keyword keyword) const;

        bool isSeparator(Separator separator) const;

        void parseTypeDeclaration();

        /// @brief Parse function declaration and return name and byte info info
        /// @param typeId Id of the type that the function belongs to. Used only for generating debug info. -1 if it belongs to "scene" type
        /// @return Tuple containing function name and runnable info
        std::pair<std::string, Engine::Runnable::RunnableFunction> parseFunctionDeclaration(size_t typeId = (size_t)-1);

        void parseInstruction(FusionInstruction instruction, Debug::FunctionDebugInfo &debugInfo);

        /// @brief Convert instruction argument into bytes that can be written to the bytecode storage
        /// @param token
        /// @return
        std::vector<uint8_t> createInstructionConstValueBytes(Token const *token);

        void consumeKeyword(Keyword keyword, std::string const &errorMessage);

        void consumeSeparator(Separator separator, std::string const &errorMessage);

        void consumeIdSeparator(std::string const &id, std::string const &errorMessage);

        /// @brief Generate storable constant of a type that can be stored on stack for usage during object initialisation
        /// @param errorMessage
        /// @return
        Engine::Runnable::CodeConstantValue parseConstant(std::string const &errorMessage);

        /// @brief Try to consume separator and if the current value is not separator of needed value, do nothing. useful for skipping line breaks
        /// @param separator
        /// @param errorMessage
        void optionallyConsumeSeparator(Separator separator);

        void advance() { m_it++; }

        void backtrack();

        CodeBuilder const &getBuilder() const { return m_builder; }

        void consumeEndOfStatement();

        void consumeEndOfStatementOrError(std::string const &err);

    private:
        void error(std::string const &errorMessage);
        CodeBuilder m_builder;
        std::vector<std::unique_ptr<Token>> m_tokens;

        std::vector<std::unique_ptr<Token>>::const_iterator m_it;
    };

}