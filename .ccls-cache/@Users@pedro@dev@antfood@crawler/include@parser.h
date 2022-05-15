#ifndef PARSER_H
#define PARSER_H
#include "../include/doctest.h"
#include <regex>
#include <array>
#include <iostream>
#include <vector>

#define REGEX_LEN 10

enum field_type {name, date, key, bpm, ts, composer, sep, ext, end_of_line, no_match};

using RegexPair = const std::pair<const field_type, const std::basic_regex<char> >;

struct Token
{
  public:
    field_type m_type;
    std::string m_value;
    Token(field_type type, const std::string &value);
    Token(field_type type, const char *value);
};

class Parser
{
  std::array<RegexPair, REGEX_LEN> m_regexes;
  std::string m_filename;
  std::string m_delimiter;
  size_t m_cursor;
  size_t m_length;

  public:
  Parser(const std::string &filename);

  std::vector<std::string> split();

static std::string type_to_string(field_type type)
{
  switch(type)
  {
    case name:
      return std::string("name");

    case date:
      return std::string("date");

    case key:
      return std::string("key");

    case bpm:
      return std::string("bpm");

    case ts:
      return std::string("time-signature");

    case composer:
      return std::string("composer");

    case sep:
      return std::string("separator");

    case ext:
      return std::string("extension");

    case end_of_line:
      return std::string("end of line");

    case no_match:
      return std::string("no match");

  };
};
};



#endif
