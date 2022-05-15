#include "../include/parser.hpp"

RegexPair NAME        (name, std::regex("^[a-zA-Z1-9]+"));
RegexPair DATE        (date, std::regex("^\\d{6}"));
RegexPair KEY         (key, std::regex("^[CDEFGAB](?:m||modal)"));
RegexPair BPM         (bpm, std::regex("^[CDEFGAB](?:m||modal)"));
RegexPair TS          (ts, std::regex("^\\d{1,2}-\\d{1,2}"));
RegexPair COMPOSER    (composer, std::regex("^[a-zA-Z]{2,3}"));
RegexPair SEP         (sep, std::regex("^_"));
RegexPair EXT         (ext, std::regex("^\\.[a-zA-Z]+"));

Token::Token(field_type type, const std::string &value):
m_type(type),
m_value(std::move(value))
{}

Token::Token(field_type type, const char *value):
m_type(type),
m_value(value)
{}

Parser::Parser(const std::string &delimiter) :
m_regexes({DATE, KEY, BPM, TS, COMPOSER, EXT, SEP, NAME}),
m_delimiter(delimiter),
m_cursor(0),
m_length(filename.length())
{ };

std::vector<std::string> Parser::split(const std::string &filename)
{
  std::vector<std::string> output;

  while(1)
  {
  size_t pos = filename.find(m_delimiter, m_cursor);

  if(pos == std::string::npos)
  {
      size_t dot_pos = filename.find(".", m_cursor);
      output.push_back(filename.substr(m_cursor, dot_pos - m_cursor ));
      m_cursor = dot_pos;
      output.push_back(filename.substr(m_cursor, m_length));
      break;
  };

  output.push_back(filename.substr(m_cursor, pos - m_cursor));
  m_cursor = (pos + m_delimiter.length());
  }

  return output;
};


//TEST_CASE("lexer")
//{
//   Lexer lexer("client_project_trackName_RecordingDescritor_120223_F_60_4-4_NXT");
//
//   while(1)
//    {
//      Token token = lexer.next_token();
//      std::cout << "value -> " << token.m_value << " | type -> " << Lexer::type_to_string(token.m_type) << std::endl;
//
//      if(token.m_type == end_of_line)
//        break;
//    };
//


//};

