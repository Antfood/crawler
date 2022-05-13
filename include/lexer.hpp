#ifndef LEXER_H
#define LEXER_H
#include <regex>
#include <array>
#include <iostream>

#define REGEX_LEN 10

enum field_type {client, project, name, descriptor, date, key, bpm, ts, composer, talent, end_of_line, no_match};

using RegexPair = const std::pair<const field_type, const std::basic_regex<char> >;

struct Token
{
  public:
  field_type m_type;
  std::string m_value;

   Token(field_type type, std::string &value);
   Token(field_type type, const char *value);
};

class Lexer
  {
    std::array<RegexPair, REGEX_LEN> m_regexes;
    std::string m_filename;
    std::string m_delimiter;
    int m_cursor;
    int m_length;

  public:
    Lexer(const std::string &filename);

    Token next_token();


  };
#endif
