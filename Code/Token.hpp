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

        size_t getRow() const { return m_row; }
        size_t getColumn() const { return m_column; }

    private:
        size_t m_row;
        size_t m_column;
    };

    class KeywordToken : public Token
    {
    public:
        explicit KeywordToken(size_t row, size_t column, Keyword keyword) : Token(row, column), m_keyword(keyword) {}
        std::string toString() const override;

        Keyword getKeyword() const { return m_keyword; }

    private:
        Keyword m_keyword;
    };

    class InstructionToken : public Token
    {
    public:
        explicit InstructionToken(size_t row, size_t column, FusionInstruction instruction) : Token(row, column), m_instruction(instruction) {}

        std::string toString() const override;
        FusionInstruction getInstruction() const { return m_instruction; }

    private:
        FusionInstruction m_instruction;
    };

    class SeparatorToken : public Token
    {
    public:
        explicit SeparatorToken(size_t row, size_t column, Separator separator) : Token(row, column), m_separator(separator) {}

        std::string toString() const override;

        Separator getSeparator() const { return m_separator; }

    private:
        Separator m_separator;
    };

    class StringToken : public Token
    {
    public:
        explicit StringToken(size_t row, size_t column, std::string const &str) : Token(row, column), m_str(str) {}

        std::string toString() const override;

        std::string const &getString() const { return m_str; }

    private:
        std::string m_str;
    };

    class IdToken : public Token
    {
    public:
        explicit IdToken(size_t row, size_t column, std::string const &str) : Token(row, column), m_str(str) {}

        std::string toString() const override;

        std::string const &getId() const { return m_str; }

    private:
        std::string m_str;
    };

     class LabelToken : public Token
    {
    public:
        explicit LabelToken(size_t row, size_t column, std::string const &str) : Token(row, column), m_str(str) {}

        std::string toString() const override;

        std::string const &getId() const { return m_str; }

    private:
        std::string m_str;
    };

    class AssetRefToken : public Token
    {
    public:
        explicit AssetRefToken(size_t row, size_t column, std::string const &assetName) : Token(row, column), m_assetName(assetName) {}

        std::string toString() const override;

        std::string const &getAssetName() const { return m_assetName; }

    private:
        std::string m_assetName;
    };

    class IntToken : public Token
    {
    public:
        explicit IntToken(size_t row, size_t column, int64_t number) : Token(row, column), m_number(number) {}
        std::string toString() const override;

        int64_t getValue() const { return m_number; }

    private:
        int64_t m_number;
    };

    class FloatToken : public Token
    {
    public:
        explicit FloatToken(size_t row, size_t column, double number) : Token(row, column), m_number(number) {}
        std::string toString() const override;

        double getValue() const { return m_number; }

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