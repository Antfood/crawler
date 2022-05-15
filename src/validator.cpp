#include "../include/validator.hpp"

Changeset::Changeset(std::vector<std::string> &&fields) :
  m_fields(fields),
  m_valid(true)
{ };

const std::string Changeset::error_to_string(const int pos)
{
  error_type err = m_errors.at(pos).first;

  switch(err)
  {
    case bad_delimiter:
      return std::string("Bad Delimiter");

    case bad_field_count:
      return std::string("Bad Field count");
    case bad_client:

      return std::string("Bad Client Name");

    case bad_project:
      return std::string("Bad Project Name");

    case bad_name:
      return std::string("Bad Recording Name");

    case bad_descriptor:
      return std::string("Bad Recording Descriptor Name");

    case bad_date:
      return std::string("Bad Date");

    case bad_key:
      return std::string("Bad Key");

    case bad_bpm:
      return std::string("Bad BPM");

    case bad_ts:
      return std::string("Bad Time Signature");

    case bad_composer:
      return std::string("Bad Composer Name");
    
    case bad_external_talent:
      return std::string("Bad External Talent");

    case bad_extension:
      return std::string("Bad Extension");
  }

};

void Changeset::invalidate(const error_type err, int pos)
{
    m_errors.push_back(std::make_pair(err, pos));
    m_valid = false;
};

Validator::Validator(const std::string &delimiter) :
  m_delimiter(delimiter),
  m_cursor(0)
{ };

struct Validator::Private 
{
  public:
    static void split_extension(Validator &self, const std::string &filename, std::vector<std::string> &output)
    {
      size_t dot_pos = filename.find(DOT, self.m_cursor);
      output.push_back(filename.substr(self.m_cursor, dot_pos - self.m_cursor));
      self.m_cursor = dot_pos;
      output.push_back(filename.substr(self.m_cursor, filename.length()));
    };

    static bool is_not_valid_name(const std::string &name)
    {
      return !std::regex_search (name, std::regex("^[a-zA-Z1-9]+$"));
    };

    static bool is_not_valid_date(const std::string &date)
    {
      return !std::regex_search (date, std::regex("^\\d{6}$"));
    };

    static bool is_not_valid_bpm(const std::string &bpm)
    {
      return !std::regex_search (bpm, std::regex("^\\d{2,3}$"));
    };

    static bool is_not_valid_key(const std::string &key)
    {
      return !std::regex_search (key, std::regex("^[CDEFGAB](?:m||modal)"));
    };

    static bool is_not_valid_ts(const std::string &ts)
    {
      return !std::regex_search (ts, std::regex("^\\d{1,2}-\\d{1,2}$"));
    };

    static bool is_not_valid_composer(const std::string &composer)
    {
      return !std::regex_search (composer, std::regex("^[a-zA-Z]{2,3}$"));
    };

    static bool is_not_valid_extension(const std::string &extension)
    {
      return !std::regex_search (extension, std::regex("^.wav$"));
    };

};

Changeset Validator::split(const std::string &filename)
{
  std::vector<std::string> output;

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
      changeset.invalidate(bad_delimiter, pos);

    pos += field.length() + 1; /* +1 to count delim */
  };
};

void Validator::validate_fields_count(Changeset &changeset)
{
  if(!changeset.m_valid) /* do not validate invalid changeset */
    return;

  size_t field_count = changeset.m_fields.size();

  if(field_count != FIELD_COUNT)
    changeset.invalidate(bad_field_count, field_count);
};

void Validator::validate_fields(Changeset &changeset)
{
  int field_index = 0;
  int pos = 0;

  for(auto &field : changeset.m_fields)
  {
    if(field_index == client &&  Private::is_not_valid_name(field))
      changeset.invalidate(bad_client, pos);

    if(field_index == project &&  Private::is_not_valid_name(field))
      changeset.invalidate(bad_project, pos);

    if(field_index == name &&  Private::is_not_valid_name(field))
      changeset.invalidate(bad_name, pos);

    if(field_index == descritor &&  Private::is_not_valid_name(field))
      changeset.invalidate(bad_descriptor, pos);

    if(field_index == date &&  Private::is_not_valid_date(field))
      changeset.invalidate(bad_date, pos);

    if(field_index == key &&  Private::is_not_valid_key(field))
      changeset.invalidate(bad_key, pos);

    if(field_index == bpm &&  Private::is_not_valid_bpm(field))
      changeset.invalidate(bad_bpm, pos); 

    if(field_index == ts &&  Private::is_not_valid_ts(field))
      changeset.invalidate(bad_ts, pos); 

    if(field_index == composer &&  Private::is_not_valid_composer(field))
      changeset.invalidate(bad_composer, pos); 

    if(field_index == external_talent && Private::is_not_valid_name(field))
      changeset.invalidate(bad_external_talent, pos); 

    if(field_index == ext && Private::is_not_valid_extension(field))
      changeset.invalidate(bad_extension, pos); 

    pos += field.length() + 1; /* +1 to count delim */
    field_index++;
  }
};


TEST_CASE("Validator")
{
  Validator validator(DELIM);

  SUBCASE("split")
  {
    auto output = validator.split("client_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "102012", "F", "120", "4-4", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("split double delim produces empty field")
  {
    auto output = validator.split("client_project_name_original__102012_F_120_4-4_PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "", "102012", "F", "120", "4-4", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("split multiple delim produces multiple empty field")
  {
    auto output = validator.split("client_project_name_original__102012_F_120_4-4___PB_DigonesBass.wav");
    std::vector<std::string>expected = {"client", "project", "name", "original", "", "102012", "F", "120", "4-4", "", "", "PB", "DigonesBass", ".wav"};

    for(size_t i =0; i < output.m_fields.size(); i++)
      CHECK(output.m_fields[i] == expected[i]);
  };

  SUBCASE("validates valid delim")
  {
    auto changeset = validator.split("client_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == true);
    CHECK(changeset.m_errors.size() == 0);

  };

  SUBCASE("validates single bad delim")
  {
    auto changeset = validator.split("client_project_name_original__102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_delimiter);
    CHECK(changeset.m_errors.at(0).second == 29); // error at position 29 in the string.
  };

  SUBCASE("validates multiple bad delim")
  {
    auto changeset = validator.split("client_project_name_original__102012_F_120__4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_delimiter);
    CHECK(changeset.m_errors.at(0).second == 29); // error at position 29 in the string.
                                                  //
    CHECK(changeset.m_errors.at(1).first == bad_delimiter);
    CHECK(changeset.m_errors.at(1).second == 43); // error at position 39 in the string.
  };

  SUBCASE("validates bad field count")
  {
    auto changeset = validator.split("client_project_name_original_FOGS_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_field_count);
    CHECK(changeset.m_errors.at(0).second == 12); // this error returns the number of fields
  };

  SUBCASE("validates bad client")
  {
    auto changeset = validator.split("cli/ent_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_client);
    CHECK(changeset.m_errors.at(0).second == 0); 
  };

  SUBCASE("validates bad project")
  {
    auto changeset = validator.split("client_proj-ect_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_project);
    CHECK(changeset.m_errors.at(0).second == 7); 
  };

  SUBCASE("validates bad recording name")
  {
    auto changeset = validator.split("client_project_na++me_original_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_name);
    CHECK(changeset.m_errors.at(0).second == 15); 
  }

  SUBCASE("validates bad recording descriptor")
  {
    auto changeset = validator.split("client_project_name_origi][nal_102012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_descriptor);
    CHECK(changeset.m_errors.at(0).second == 20); 
  }

  SUBCASE("validates bad date")
  {
    auto changeset = validator.split("client_project_name_original_10299012_F_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_date);
    CHECK(changeset.m_errors.at(0).second == 29); 
  }

  SUBCASE("validates bad key")
  {
    auto changeset = validator.split("client_project_name_original_102012_N_120_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_key);
    CHECK(changeset.m_errors.at(0).second == 36); 
  }

  SUBCASE("validates bad bpm")
  {
    auto changeset = validator.split("client_project_name_original_102012_G_1290_4-4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_bpm);
    CHECK(changeset.m_errors.at(0).second == 38); 
  }

  SUBCASE("validates bad time signature")
  {
    auto changeset = validator.split("client_project_name_original_102012_G_129_4/4_PB_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_ts);
    CHECK(changeset.m_errors.at(0).second == 42); 
  }


  SUBCASE("validates bad composer initials")
  {
    auto changeset = validator.split("client_project_name_original_102012_G_129_4-4_P_DigonesBass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_composer);
    CHECK(changeset.m_errors.at(0).second == 46); 
  }

  SUBCASE("validates bad talent")
  {
    auto changeset = validator.split("client_project_name_original_102012_G_129_4-4_WB_Digones--Bass.wav");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_external_talent);
    CHECK(changeset.m_errors.at(0).second == 49); 
  }

  SUBCASE("validates bad extension")
  {
    auto changeset = validator.split("client_project_name_original_102012_G_129_4-4_PB_DigonesBass.mp3");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_valid == false);
    CHECK(changeset.m_errors.at(0).first == bad_extension);
    CHECK(changeset.m_errors.at(0).second == 61); 
  }

  SUBCASE("validate multiple bad fields")
  {
    auto changeset = validator.split("client_project_na./me_original_1020012_G_1290_4-4_PB_DigonesBass.mp3");
    validator.validate_delimiter(changeset);
    validator.validate_fields_count(changeset);
    validator.validate_fields(changeset);

    CHECK(changeset.m_errors.at(0).first == bad_name);
    CHECK(changeset.m_errors.at(1).first == bad_date);
    CHECK(changeset.m_errors.at(2).first == bad_bpm);
  }

};

