#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <regex>
#include <array>

#define REGEX_LEN 10

enum field_type {client, project, name, descriptor, date, key, bpm, ts, composer, talent};

using RegexPair = const std::pair<const field_type, const std::basic_regex<char> >;

class Validator
  {
    std::array<RegexPair, REGEX_LEN> m_regexes;

  public:
    Validator();


  };


#endif
