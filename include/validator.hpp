#ifndef PARSER_H
#define PARSER_H
#include "../include/doctest.h"
#include <regex>
#include <array>
#include <iostream>
#include <vector>

#define REGEX_LEN 10
#define DOT "."
#define DELIM "_"
#define FIELD_COUNT 10 /* 9 fields + extention */

enum field_type {name, date, key, bpm, ts, composer, sep, ext, end_of_line, no_match};
enum error_type {nil, bad_delimiter, bad_field_count};

using ValidationError = std::pair<const error_type, const size_t>;
using RegexPair = const std::pair<const field_type, const std::basic_regex<char> >;

struct Changeset
{
  public:
  std::vector<const std::string> m_fields;
  std::vector<ValidationError> m_errors;
  bool m_valid;

  Changeset(std::vector<const std::string> &&fields);

   const std::string error_to_string(const int pos);
};

class Validator
{
  std::array<RegexPair, REGEX_LEN> m_regexes;
  std::string                      m_delimiter;
  size_t                           m_cursor;
  struct                           Private;

  public:
  Validator(const std::string &delimeter);

  Changeset                  split(const std::string &filename);
  void                       validate_delimiter(Changeset &changeset);
  void                       validate_fields_count(Changeset &changeset);
  static std::string         type_to_string(field_type type);

};

#endif
