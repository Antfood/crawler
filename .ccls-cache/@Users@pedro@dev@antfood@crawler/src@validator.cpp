#include "../include/validator.hpp"

RegexPair NAME        (name, std::regex("^[a-zA-Z1-9]+"));
RegexPair DATE        (date, std::regex("^\\d{6}"));
RegexPair KEY         (key, std::regex("^[CDEFGAB](?:m||modal)"));
RegexPair BPM         (bpm, std::regex("^[CDEFGAB](?:m||modal)"));
RegexPair TS          (ts, std::regex("^\\d{1,2}-\\d{1,2}"));
RegexPair COMPOSER    (composer, std::regex("^[a-zA-Z]{2,3}"));
RegexPair SEP         (sep, std::regex("^_"));
RegexPair EXT         (ext, std::regex("^\\.[a-zA-Z]+"));

Changeset::Changeset(std::vector<const std::string> &&fields) :
  m_fields(fields),
  m_valid(true)
{ };


const std::string Changeset::error_to_string(const int pos)
{
  error_type err = m_errors.at(pos).first;

  switch(err)
  {
    case nil:
      return std::string("nil");

    case bad_delimiter:
      return std::string("Bad Delimiter");

    case bad_field_count:
      return std::string("Bad Field count");
  };

};

Validator::Validator(const std::string &delimiter) :
  m_regexes({DATE, KEY, BPM, TS, COMPOSER, EXT, SEP, NAME}),
  m_delimiter(delimiter),
  m_cursor(0)
{ };

struct Validator::Private 
{
  public:
    static void split_extension(Validator &self, const std::string &filename, std::vector<const std::string> &output)
    {
      size_t dot_pos = filename.find(DOT, self.m_cursor);
      output.push_back(filename.substr(self.m_cursor, dot_pos - self.m_cursor));
      self.m_cursor = dot_pos;
      output.push_back(filename.substr(self.m_cursor, filename.length()));
    };
};

Changeset Validator::split(const std::string &filename)
{
  std::vector<const std::string> output;

  while(1)
  {
    size_t pos = filename.find(m_delimiter, m_cursor);

    if(pos == std::string::npos)
    {
      Private::split_extension(*this, filename, output);
      break;
    };

    output.push_back(filename.substr(m_cursor, pos - m_cursor));
    m_cursor = (pos + m_delimiter.length());
  }

  return Changeset(std::move(output));
};

void Validator::validate_delimiter(Changeset &changeset)
{
  int pos = 0;
  
  for(auto &field : changeset.m_fields)
  {
    if(field.length() == 0)
    {
       changeset.m_errors.push_back(std::make_pair(bad_delimiter, pos));
       changeset.m_valid = false;
    }

    pos += field.length() + 1; /* +1 to count delim */
  };
};

void Validator::validate_fields_count(Changeset &changeset)
{
  if(!changeset.m_valid) /* do not validate invalid changeset */
      return;

  size_t field_count = changeset.m_fields.size();

  if(field_count != FIELD_COUNT)
   {
     changeset.m_valid = false;
     changeset.m_errors.push_back(std::make_pair(bad_field_count, field_count));
   };
};

std::string Validator::type_to_string(field_type type)
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


TEST_CASE("Validator")
{
  Validator validator(DELIM);

  SUBCASE("split")
  {
    auto output = validator.split("client_project_name_original_102012_F_4-4_PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "102012", "F", "4-4", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("split double delim produes empty field")
  {
    auto output = validator.split("client_project_name_original__102012_F_4-4_PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "", "102012", "F", "4-4", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("split multiple delim produes multiple empty field")
  {
    auto output = validator.split("client_project_name_original__102012_F_4-4___PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "", "102012", "F", "4-4", "", "", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("validates valid delim")
  {
    auto changeset = validator.split("client_project_name_original_102012_F_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == true);
    CHECK(changeset.m_errors.size() == 0);

  };

  SUBCASE("validates single bad delim")
  {
    auto changeset = validator.split("client_project_name_original__102012_F_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_delimiter);
    CHECK(changeset.m_errors.at(0).second == 29); // error at position 29 in the string.
  };

  SUBCASE("validates multiple bad delim")
  {
    auto changeset = validator.split("client_project_name_original__102012_F__4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_delimiter);
    CHECK(changeset.m_errors.at(0).second == 29); // error at position 29 in the string.
                                                  //
    CHECK(changeset.m_errors.at(1).first == bad_delimiter);
    CHECK(changeset.m_errors.at(1).second == 39); // error at position 39 in the string.
  };

  SUBCASE("validates bad field count")
  {
    auto changeset = validator.split("client_project_name_original_FOGS_102012_F_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_field_count);
    CHECK(changeset.m_errors.at(0).second == 11); // error at position 29 in the string.
  };
};

