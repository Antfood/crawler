#include "../include/changeset.hpp"

struct Changeset::Private{

  static std::string error_to_string(Changeset &self, int pos)
  {
    error_type err = self.m_errors.at (pos).first;
   int count = self.m_errors.at (pos).second;

    switch (err)
      {
        case bad_delimiter:return {"Bad Delimiter"};
        case bad_field_count:return {"\nBad Field count. Must have " + std::to_string(FIELD_COUNT - 1) + " fields but found only " + std::to_string(count - 1) + "." };
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
      }
  };

};

Changeset::Changeset (std::vector<std::string> &&fields) :
    m_fields (fields),
    m_valid (true),
    m_quit(false),
    m_cleared(false),
    m_skipped(false)
{}


std::string Changeset::first_error ()
{

  if(has_error (bad_field_count))
      return Private::error_to_string (*this, 0);

  size_t size = m_errors.at (0).second + 2;

  char buffer[size];
  memset (buffer, ' ', size);
  buffer[size - 1] = '\0';
  buffer[size - 2] = '^';

  std::string spaces = std::string (buffer);
  return spaces + Private::error_to_string (*this, 0);
}

std::string Changeset::other_errors ()
{
  std::string buffer;

  // skip first error
  for (int i = 1; i < m_errors.size (); i++)
    {
      if (i == m_errors.size () - 1)
        buffer += Private::error_to_string (*this, i);
      else
        buffer += Private::error_to_string (*this, i) + " | ";
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
  m_cleared = true;
}

void Changeset::clear_error(error_type err)
{
  auto itr = std::find_if(m_errors.begin(), m_errors.end(),[&err](const ValidationError & element){ return element.first == err;} );

  if(itr != m_errors.cend())
    {
      auto err_index = std::distance (m_errors.begin (), itr);
      m_errors.erase (std::next (m_errors.begin (), err_index));
    }
}

bool Changeset::error_is_bad_field_count ()
{
  return !m_valid && m_errors.at(0).first == bad_field_count;
}

void Changeset::fill_empty_fields ()
{
  Changeset::fill_fields (m_fields);
}

void Changeset::fill_fields (std::vector<std::string> &fields)
{
  size_t fields_length = fields.size();

  if(fields_length == FIELD_COUNT)
    return;

  auto last_field = fields.at(fields_length - 1);
  bool is_extension = std::regex_search(last_field, std::regex("^.[a-zA-Z]{2,3}"));

  if(is_extension)
    fields.pop_back();

  while (fields_length <= FIELD_COUNT)
    {
      if (is_extension && fields_length == FIELD_COUNT)
        fields.emplace_back (last_field); /* fill fields with empty strings */
      else
        fields.emplace_back (std::string ("?")); /* fill fields with empty strings */

      fields_length++;
    }
}

std::string Changeset::build_path()
{
  std::string result;
  int count = 0;

  for(auto &field : m_fields)
  {
    result += field;
     if(count < FIELD_COUNT - 2) /* do not add delim for extension */
       result += DELIM;
    count++;
  }

  return result;
}