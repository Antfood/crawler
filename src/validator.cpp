#include "../include/validator.hpp"
#include <exception>
#include <utility>

Changeset::Changeset (std::vector<std::string> &&fields) :
    m_fields (fields),
    m_valid (true),
    m_quit(false),
    m_was_cleared(false)
{}

std::string Changeset::error_to_string (const int pos)
{
  error_type err = m_errors.at (pos).first;

  switch (err)
    {
      case bad_delimiter:return {"Bad Delimiter"};
      case bad_field_count:return {"Bad Field count"};
      case bad_client:return {"Bad Client Name"};
      case bad_project:return {"Bad Project Name"};
      case bad_name:return {"Bad Recording Name"};
      case bad_descriptor:return {"Bad Recording Descriptor Name"};
      case bad_date:return {"Bad Date"};
      case bad_key:return {"Bad Key"};
      case bad_bpm:return {"Bad BPM"};
      case bad_ts:return {"Bad Time Signature"};
      case bad_composer:return {"Bad Composer Name"};
      case bad_external_talent:return {"Bad External Talent"};
      case bad_extension:return {"Bad Extension"};

      default:return {"Unexpected Error"};
    }
}

std::string Changeset::first_error ()
{
  size_t size = m_errors.at (0).second + 2;

  char buffer[size];
  memset (buffer, ' ', size);
  buffer[size - 1] = '\0';
  buffer[size - 2] = '^';

  std::string spaces = std::string (buffer);

  return spaces + error_to_string (0);
}

std::string Changeset::other_errors ()
{
  std::string buffer;

  // skip first error
  for (int i = 1; i < m_errors.size (); i++)
    {
      if (i == m_errors.size () - 1)
        buffer += error_to_string (i);
      else
        buffer += error_to_string (i) + " | ";
    }
  return buffer;
}

void Changeset::invalidate (const error_type err, int pos)
{
  m_errors.emplace_back (std::make_pair (err, pos));
  m_valid = false;
}

bool Changeset::has_error(error_type err)
{
  return std::any_of(m_errors.begin(), m_errors.end(), [err](auto &error){ return error.first == err;});
}

void Changeset::clear_errors ()
{
  m_errors.clear();
  m_valid = true;
  m_was_cleared = true;
}

bool Changeset::was_bad_field_count ()
{
  return m_errors.at(0).first == bad_field_count;
}

void Changeset::fill_empty_fields ()
{
  size_t fields_length = m_fields.size();

  if(fields_length == FIELD_COUNT)
     return;

  auto last_field = m_fields.at(fields_length - 1);
  bool is_extension = std::regex_search(last_field, std::regex("^.[a-zA-Z]{2,3}"));

  if(is_extension)
      m_fields.pop_back();

  while (fields_length <= FIELD_COUNT)
    {
      if (is_extension && fields_length == FIELD_COUNT)
        m_fields.emplace_back (last_field); /* fill fields with empty strings */
      else
        m_fields.emplace_back (std::string ("?")); /* fill fields with empty strings */

      fields_length++;
    }
}

struct Validator::Private {
 public:
  static void split_extension (Validator &self, const std::string &filename, std::vector<std::string> &output)
  {
    size_t dot_pos = filename.find ('.', self.m_cursor);
    output.push_back (filename.substr (self.m_cursor, dot_pos - self.m_cursor));
    self.m_cursor = dot_pos;
    output.push_back (filename.substr (self.m_cursor, filename.length ()));
  };

  static bool is_not_valid_name (const std::string &name)
  {
    return !std::regex_search (name, std::regex ("^[a-zA-Z1-9]+$"));
  };

  static bool is_not_valid_date (const std::string &date)
  {
    return !std::regex_search (date, std::regex ("^\\d{6}$"));
  };

  static bool is_not_valid_bpm (const std::string &bpm)
  {
    return !std::regex_search (bpm, std::regex ("^\\d{2,3}$"));
  };

  static bool is_not_valid_key (const std::string &key)
  {
    return !std::regex_search (key, std::regex ("^[CDEFGAB](?:m||modal)"));
  };

  static bool is_not_valid_ts (const std::string &ts)
  {
    return !std::regex_search (ts, std::regex ("^\\d{1,2}-\\d{1,2}$"));
  };

  static bool is_not_valid_composer (const std::string &composer)
  {
    return !std::regex_search (composer, std::regex ("^[a-zA-Z]{2,3}$"));
  };

  static bool is_not_valid_extension (const std::string &extension)
  {
    return !std::regex_search (extension, std::regex ("^.wav$"));
  };

};

Validator::Validator (std::string delimiter) :
    m_delimiter (std::move(delimiter)),
    m_cursor (0)
{}

Changeset Validator::split (const std::string &filename)
{
  std::vector<std::string> output;
  m_cursor = 0;

  while (true)
    {
      size_t pos = filename.find (m_delimiter, m_cursor);

      if (pos == std::string::npos)
        {
          Private::split_extension (*this, filename, output);
          break;
        }

      output.push_back (filename.substr (m_cursor, pos - m_cursor));
      m_cursor = (pos + m_delimiter.length ());

    }

  return Changeset(std::move (output));
}

void Validator::validate_delimiter (Changeset &changeset)
{
  size_t pos = 0;

  for (auto &field : changeset.m_fields)
    {
      if (field.length () == 0)
        changeset.invalidate (bad_delimiter, static_cast<int>(pos));

      pos += field.length () + 1; /* +1 to count delim */
    }
}

void Validator::validate_fields_count (Changeset &changeset)
{
  if (!changeset.m_valid) /* do not validate invalid changeset */
    return;

  size_t field_count = changeset.m_fields.size ();

  if (field_count != FIELD_COUNT)
    changeset.invalidate (bad_field_count, static_cast<int>(field_count));
}

void Validator::validate_fields (Changeset &changeset)
{

  if (!changeset.m_valid) /* do not validate invalid changeset */
    return;

  int field_index = 0;
  int pos = 0;

  for (auto &field : changeset.m_fields)
    {
      if (field_index == client && Private::is_not_valid_name (field))
        changeset.invalidate (bad_client, pos);

      if (field_index == project && Private::is_not_valid_name (field))
        changeset.invalidate (bad_project, pos);

      if (field_index == name && Private::is_not_valid_name (field))
        changeset.invalidate (bad_name, pos);

      if (field_index == descriptor && Private::is_not_valid_name (field))
        changeset.invalidate (bad_descriptor, pos);

      if (field_index == date && Private::is_not_valid_date (field))
        changeset.invalidate (bad_date, pos);

      if (field_index == key && Private::is_not_valid_key (field))
        changeset.invalidate (bad_key, pos);

      if (field_index == bpm && Private::is_not_valid_bpm (field))
        changeset.invalidate (bad_bpm, pos);

      if (field_index == ts && Private::is_not_valid_ts (field))
        changeset.invalidate (bad_ts, pos);

      if (field_index == composer && Private::is_not_valid_composer (field))
        changeset.invalidate (bad_composer, pos);

      if (field_index == external_talent && Private::is_not_valid_name (field))
        changeset.invalidate (bad_external_talent, pos);

      if (field_index == ext && Private::is_not_valid_extension (field))
        changeset.invalidate (bad_extension, pos);

      pos += static_cast<int>(field.length ()) + 1; /* +1 to count delim */
      field_index++;
    }
}

TEST_CASE("Validator")
{
  Validator validator (DELIM);

  SUBCASE("split")
    {
      auto output = validator.split ("client_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
      std::vector<std::string> expected = {"client", "project", "name", "original", "102012", "F", "120", "4-4", "PB",
                                           "DigonesBass", ".wav"};

      for (size_t i = 0; i < output.m_fields.size (); i++)
        CHECK(output.m_fields[i] == expected[i]);
    }

  SUBCASE("split double delim produces empty field")
    {
      auto output = validator.split ("client_project_name_original__102012_F_120_4-4_PB_DigonesBass.wav");
      std::vector<std::string> expected = {"client", "project", "name", "original", "", "102012", "F", "120", "4-4",
                                           "PB", "DigonesBass", ".wav"};

      for (size_t i = 0; i < output.m_fields.size (); i++)
        CHECK(output.m_fields[i] == expected[i]);
    }

  SUBCASE("split multiple delim produces multiple empty field")
    {
      auto output = validator.split ("client_project_name_original__102012_F_120_4-4___PB_DigonesBass.wav");
      std::vector<std::string> expected = {"client", "project", "name", "original", "", "102012", "F", "120", "4-4", "",
                                           "", "PB", "DigonesBass", ".wav"};

      for (size_t i = 0; i < output.m_fields.size (); i++)
        CHECK(output.m_fields[i] == expected[i]);
    }

  SUBCASE("validates valid delim")
    {
      auto changeset = validator.split ("client_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);

      CHECK(changeset.m_valid == true);
      CHECK(changeset.m_errors.size () == 0);

    }

  SUBCASE("validates single bad delim")
    {
      auto changeset = validator.split ("client_project_name_original__102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_delimiter);
      CHECK(changeset.m_errors.at (0).second == 29); // error at position 29 in the string.
    }

  SUBCASE("validates multiple bad delim")
    {
      auto changeset = validator.split ("client_project_name_original__102012_F_120__4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_delimiter);
      CHECK(changeset.m_errors.at (0).second == 29); // error at position 29 in the string.
      //
      CHECK(changeset.m_errors.at (1).first == bad_delimiter);
      CHECK(changeset.m_errors.at (1).second == 43); // error at position 39 in the string.
    }

  SUBCASE("validates bad field count")
    {
      auto changeset = validator.split ("client_project_name_original_FOGS_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_field_count);
      CHECK(changeset.m_errors.at (0).second == 12); // this error returns the number of fields
    }

  SUBCASE("validates bad client")
    {
      auto changeset = validator.split ("cli/ent_project_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_client);
      CHECK(changeset.m_errors.at (0).second == 0);
    }

  SUBCASE("validates bad project")
    {
      auto changeset = validator.split ("client_proj-ect_name_original_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_project);
      CHECK(changeset.m_errors.at (0).second == 7);
    }

  SUBCASE("validates bad recording name")
    {
      auto changeset = validator.split ("client_project_na++me_original_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_name);
      CHECK(changeset.m_errors.at (0).second == 15);
    }

  SUBCASE("validates bad recording descriptor")
    {
      auto changeset = validator.split ("client_project_name_origi][nal_102012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_descriptor);
      CHECK(changeset.m_errors.at (0).second == 20);
    }

  SUBCASE("validates bad date")
    {
      auto changeset = validator.split ("client_project_name_original_10299012_F_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_date);
      CHECK(changeset.m_errors.at (0).second == 29);
    }

  SUBCASE("validates bad key")
    {
      auto changeset = validator.split ("client_project_name_original_102012_N_120_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_key);
      CHECK(changeset.m_errors.at (0).second == 36);
    }

  SUBCASE("validates bad bpm")
    {
      auto changeset = validator.split ("client_project_name_original_102012_G_1290_4-4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_bpm);
      CHECK(changeset.m_errors.at (0).second == 38);
    }

  SUBCASE("validates bad time signature")
    {
      auto changeset = validator.split ("client_project_name_original_102012_G_129_4/4_PB_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_ts);
      CHECK(changeset.m_errors.at (0).second == 42);
    }

  SUBCASE("validates bad composer initials")
    {
      auto changeset = validator.split ("client_project_name_original_102012_G_129_4-4_P_DigonesBass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_composer);
      CHECK(changeset.m_errors.at (0).second == 46);
    }

  SUBCASE("validates bad talent")
    {
      auto changeset = validator.split ("client_project_name_original_102012_G_129_4-4_WB_Digones--Bass.wav");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_external_talent);
      CHECK(changeset.m_errors.at (0).second == 49);
    }

  SUBCASE("validates bad extension")
    {
      auto changeset = validator.split ("client_project_name_original_102012_G_129_4-4_PB_DigonesBass.mp3");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_valid == false);
      CHECK(changeset.m_errors.at (0).first == bad_extension);
      CHECK(changeset.m_errors.at (0).second == 61);
    }

  SUBCASE("validate multiple bad fields")
    {
      auto changeset = validator.split ("client_project_na./me_original_1020012_G_1290_4-4_PB_DigonesBass.mp3");
      validator.validate_delimiter (changeset);
      validator.validate_fields_count (changeset);
      validator.validate_fields (changeset);

      CHECK(changeset.m_errors.at (0).first == bad_name);
      CHECK(changeset.m_errors.at (1).first == bad_date);
      CHECK(changeset.m_errors.at (2).first == bad_bpm);
    }

  SUBCASE("Edge case")
    {

      auto changeset = validator.split ("BumbleBee_Anthem_TuneAPiano_ChillVersion__220229_Bb_105_4-4_BE_NXT.wavcursor");

    }
}