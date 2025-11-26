#pragma once
#include <cstdint>
#include <string>
#include <map>
#include "Lexems.hpp"
namespace Code
{
    class Token
    {
    public:
        explicit Token(size_t row, size_t column) : m_row(row), m_column(column) {}
        virtual std::string toString() const = 0;

    private:
        size_t m_row;
        size_t m_column;
    };

    class KeywordToken : public Token
    {
    public:
        explicit KeywordToken(size_t row, size_t column, Keyword keyword) : Token(row, column), m_keyword(keyword) {}
        std::string toString() const override;

    private:
        Keyword m_keyword;
    };

    class InstructionToken : public Token
    {
    public:
        explicit InstructionToken(size_t row, size_t column, FusionInstruction instruction) : Token(row, column), m_instruction(instruction) {}

        std::string toString() const override;

    private:
        FusionInstruction m_instruction;
    };

    class SeparatorToken : public Token
    {
    public:
        explicit SeparatorToken(size_t row, size_t column, Separator separator) : Token(row, column), m_separator(separator) {}

        std::string toString() const override;

    private:
        Separator m_separator;
    };

    class StringToken : public Token
    {
    public:
        explicit StringToken(size_t row, size_t column, std::string const &str) : Token(row, column), m_str(str) {}

        std::string toString() const override;

    private:
        std::string m_str;
    };

    class IdToken : public Token
    {
    public:
        explicit IdToken(size_t row, size_t column, std::string const &str) : Token(row, column), m_str(str) {}

        std::string toString() const override;

    private:
        std::string m_str;
    };

    class AssetRefToken : public Token
    {
    public:
        explicit AssetRefToken(size_t row, size_t column, std::string const &assetName) : Token(row, column), m_assetName(assetName) {}

        std::string toString() const override;

    private:
        std::string m_assetName;
    };

    class IntToken : public Token
    {
    public:
        explicit IntToken(size_t row, size_t column, int64_t number) : Token(row, column), m_number(number) {}
        std::string toString() const override;

    private:
        int64_t m_number;
    };

    class FloatToken : public Token
    {
    public:
        explicit FloatToken(size_t row, size_t column, double number) : Token(row, column), m_number(number) {}
        std::string toString() const override;

    private:
        double m_number;
    };

    class BoolToken : public Token
    {
        explicit BoolToken(size_t row, size_t column, bool val) : Token(row, column), m_val(val) {}
        std::string toString() const override;

    private:
        bool m_val;
    };
}