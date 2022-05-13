#include "../include/lexer.hpp"

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

Token::Token(field_type type, std::string &value):
m_type(type),
m_value(std::move(value))
{}

Token::Token(field_type type, const char *value):
m_type(type),
m_value(value)
{}


Lexer::Lexer(const std::string &filename) :
m_regexes({CLIENT, PROJECT, NAME, DESCRIPTOR, DATE, KEY, BPM, TS, COMPOSER, TALENT}),
m_filename(filename),
m_delimiter("_"),
m_cursor(0),
m_length(filename.length())
{ };


Token Lexer::next_token()
{
  //std::string res = m_filename.substr(0, m_filename.find(m_delimiter));

  if(m_cursor == m_length)
      return Token(end_of_line, "");
        
};
