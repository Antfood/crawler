#include "../include/validator.hpp"

const std::array<std::regex, REGEX_COUNT> REGEXES = {
    std::regex ("^[a-zA-Z1-9]+$"),                            /* client, project, composition*/
    std::regex ("^[a-zA-Z]{4,}$"),                            /*  descriptor minimum of 4 letters */
    std::regex ("^\\d{6}$"),                                  /* date */
    std::regex ("^[CDEFGAB](?:#||b)(?:m||modal)$"),           /* key  */
    std::regex ("^\\d{2,3}$"),                                /* bpm  */
    std::regex ("^\\d{1,2}-\\d{1,2}$|^multiple"),             /* time signature */
    std::regex ("^(?!NXT)[A-Z]{2,3}((-[A-Z]{2,3})?)+$"),      /* composer - ignores NXT  */
    std::regex (R"(^NXT|(?:\w+\(\w+\))+)"),                   /* talent  */
    std::regex ("^.wav$")
};

struct Validator::Private {
 public:
  static void split_extension (Validator &self, const std::string &filename, std::vector<std::string> &output)
  {
    size_t dot_pos = filename.find ('.', self.m_cursor);
    output.push_back (filename.substr (self.m_cursor, dot_pos - self.m_cursor));
    self.m_cursor = dot_pos;
    output.push_back (filename.substr (self.m_cursor, filename.length ()));
  };

  static bool is_valid_name (const std::string &name)
  {
    return std::regex_search (name, REGEXES.at (name_regex));
  };

  static bool is_valid_descriptor (const std::string &descriptor)
  {
    return std::regex_search (descriptor, REGEXES.at (descriptor_regex));
  };

  static bool is_valid_date (const std::string &date)
  {
    return std::regex_search (date, REGEXES.at (date_regex));
  };

  static bool is_valid_bpm (const std::string &bpm)
  {
    return std::regex_search (bpm, REGEXES.at (bpm_regex));
  };

  static bool is_valid_key (const std::string &key)
  {
    return std::regex_search (key, REGEXES[key_regex]);
  };

  static bool is_valid_ts (const std::string &ts)
  {
    return std::regex_search (ts, REGEXES[ts_regex]);
  };

  static bool is_valid_composer (const std::string &composer)
  {
    return std::regex_search (composer, REGEXES.at (composer_regex));
  };

  static bool is_valid_talent (const std::string &talent)
  {
    return std::regex_search (talent, REGEXES.at (talent_regex));
  }

  static bool is_valid_extension (const std::string &extension)
  {
    return std::regex_search (extension, REGEXES.at (ext_regex));
  };

  static std::vector<std::string> create_new_fields (Changeset &changeset)
  {
    std::vector<std::string> new_fields = {"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?"};
    int field_index = 0;
    int pos = 0;

    for (auto &field : changeset.m_fields)
      {
        if (field_index == client)
          {
            if (Private::is_valid_name (field))
              new_fields[client] = field;
            else
              changeset.invalidate (bad_client, pos);
          }
        if (field_index == project)
          {
            if (Private::is_valid_name (field))
              new_fields[project] = field;
            else
              changeset.invalidate (bad_project, pos);
          }
        if (field_index == recording)
          {
            if (Private::is_valid_name (field))
              new_fields[recording] = field;
            else
              changeset.invalidate (bad_descriptor, pos);
          }
        if (field_index == descriptor)
          {
            if (Private::is_valid_descriptor (field))
              new_fields[descriptor] = field;
            else
              changeset.invalidate (bad_descriptor, pos);
          }
        if (field_index == date)
          {
            if (Private::is_valid_date (field))
              new_fields[date] = field;
            else
              changeset.invalidate (bad_date, pos);
          }
        if (field_index == key)
          {
            if (Private::is_valid_key (field))
              new_fields[key] = field;
            else
              changeset.invalidate (bad_key, pos);
          }
        if (field_index == bpm)
          {
            if (Private::is_valid_bpm (field))
              new_fields[bpm] = field;
            else
              changeset.invalidate (bad_bpm, pos);
          }
        if (field_index == ts)
          {
            if (Private::is_valid_ts (field))
              new_fields[ts] = field;
            else
              changeset.invalidate (bad_ts, pos);
          }
        if (field_index == composer)
          {
            if (Private::is_valid_composer (field))
              new_fields[composer] = field;
            else
              changeset.invalidate (bad_composer, pos);
          }
        if (field_index == external_talent)
          {
            if (Private::is_valid_talent (field))
              new_fields[external_talent] = field;
            else
              changeset.invalidate (bad_external_talent, pos);
          }
        if (field_index == ext)
          {
            if (Private::is_valid_extension (field))
              new_fields[ext] = field;
            else
              changeset.invalidate (bad_extension, pos);
          }

        field_index++;
        pos += static_cast<int>(field.length ()) + 1; /* +1 to count delim */
      }

    return new_fields;
  }

  static int
  find_suitable_field (const Changeset &changeset, const std::vector<std::string> &new_fields, regex_type rtype)
  {
    for(int index = FIELD_COUNT - 1; index >= 0; index--)
      {
        bool is_used = (std::find (new_fields.begin (), new_fields.end (), changeset.m_fields.at(index)) != new_fields.end ());

        if (std::regex_search (changeset.m_fields.at(index), REGEXES.at (rtype)) && !is_used ) // check if field is already being used
          return index;
      }

    return -1;
  }

  static void redistribute_fields_and_clear_errors(Changeset &changeset, std::vector<std::string> &new_fields)
  {

    int index = 0;
    std::vector<error_type> errs_to_clear;

    for (auto &err : changeset.m_errors)
      {
        if (err.first < 2)
          continue;

        regex_type rtype =
            err.first <= bad_name ? name_regex : static_cast<regex_type>(err.first - REGEX_FIELD_ERR_FIELD_OFFSET);
        auto field_index = Private::find_suitable_field (changeset, new_fields, rtype);

        if (field_index > 0)
          {
            new_fields[err.first - TYPE_FIELD_ERR_FIELD_OFFSET] = changeset.m_fields.at (field_index);
            errs_to_clear.emplace_back (err.first);
          }
      }

    for(auto &err : errs_to_clear)
      changeset.clear_error (err);
  }

};

Validator::Validator (std::string delimiter) :
    m_delimiter (std::move (delimiter)),
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

  return Changeset (std::move (output));
}

void Validator::fix_bad_fields_count (Changeset &changeset)
{
  if (!changeset.error_is_bad_field_count ()) /* fix only on bad field count */
    return;

  std::vector<std::string> new_fields = Private::create_new_fields (changeset);
  Changeset::fill_fields (new_fields);
  Private::redistribute_fields_and_clear_errors (changeset, new_fields);

  changeset.m_fields = new_fields;
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
      if (field_index == client && !Private::is_valid_name (field))
        changeset.invalidate (bad_client, pos);

      if (field_index == project && !Private::is_valid_name (field))
        changeset.invalidate (bad_project, pos);

      if (field_index == recording && !Private::is_valid_name (field))
        changeset.invalidate (bad_name, pos);

      if (field_index == descriptor && !Private::is_valid_descriptor (field))
        changeset.invalidate (bad_descriptor, pos);

      if (field_index == date && !Private::is_valid_date (field))
        changeset.invalidate (bad_date, pos);

      if (field_index == key && !Private::is_valid_key (field))
        changeset.invalidate (bad_key, pos);

      if (field_index == bpm && !Private::is_valid_bpm (field))
        changeset.invalidate (bad_bpm, pos);

      if (field_index == ts && !Private::is_valid_ts (field))
        changeset.invalidate (bad_ts, pos);

      if (field_index == composer && !Private::is_valid_composer (field))
        changeset.invalidate (bad_composer, pos);

      if (field_index == external_talent && !Private::is_valid_talent (field))
        changeset.invalidate (bad_external_talent, pos);

      if (field_index == ext && !Private::is_valid_extension (field))
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