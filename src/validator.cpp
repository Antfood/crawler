#include "../include/validator.hpp"

RegexPair CLIENT      (client, std::regex("^[a-zA-Z1-9]+"));
RegexPair PROJECT     (project, std::regex("^[a-zA-Z1-9]+"));
RegexPair NAME        (client, std::regex("^[a-zA-Z1-9]+"));
RegexPair DESCRIPTOR  (descriptor, std::regex("^[a-zA-Z1-9]+"));
RegexPair DATE        (date, std::regex("^\\d{6}$"));
RegexPair KEY         (key, std::regex("^[CDEFGAB](?:m||modal)$"));
RegexPair BPM         (bpm, std::regex("^[CDEFGAB](?:m||modal)$"));
RegexPair TS          (ts, std::regex("^\\d{1,2}-\\d{1,2}$"));
RegexPair COMPOSER    (composer, std::regex("^[a-zA-Z]{2,3}$"));
RegexPair TALENT      (talent, std::regex("^[a-zA-Z]+.[a-zA-Z]+$"));


Validator::Validator() :
m_regexes({CLIENT, PROJECT, NAME, DESCRIPTOR, DATE, KEY, BPM, TS, COMPOSER, TALENT})
{ };
