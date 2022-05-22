#ifndef PARSER_H
#define PARSER_H
#include "../include/doctest.h"
#include <regex>
#include <array>
#include <iostream>
#include <vector>

#define DELIM "_"
#define FIELD_COUNT 11  /* 10 fields + extension */

enum field_type 
{
  client           = 0,
  project          = 1,
  name             = 2,
  descriptor       = 3,
  date             = 4,
  key              = 5,
  bpm              = 6,
  ts               = 7,
  composer         = 8,
  external_talent  = 9,
  ext              = 10
};
enum error_type
{
  bad_delimiter       = 0,
  bad_field_count     = 1,
  bad_client          = 2,
  bad_project         = 3,
  bad_name            = 4,
  bad_descriptor      = 5,
  bad_date            = 6,
  bad_key             = 7,
  bad_bpm             = 8,
  bad_ts              = 9,
  bad_composer        = 10,
  bad_external_talent = 11,
  bad_extension       = 12
};

using ValidationError = std::pair<const error_type, const size_t>;

struct Changeset
{
  public:
    std::vector<std::string>       m_fields;
    std::vector<ValidationError>   m_errors;
    bool                           m_valid;
    bool                           m_was_cleared;
    bool                           m_quit;

  explicit Changeset(std::vector<std::string> &&fields);

    std::string             error_to_string(int pos);
    std::string             other_errors();
    std::string             first_error();
    void                    fill_empty_fields();
    void                    invalidate(error_type err, int pos);
    bool                    has_error(error_type err);
    void                    clear_errors();
    bool                    was_bad_field_count();
};

class Validator
{
  std::string                      m_delimiter;
  size_t                           m_cursor;
  struct                           Private;

  public:
  explicit Validator(std::string delimeter);

  Changeset                  split(const std::string &filename);
  static void                validate_fields(Changeset &changeset);
  static void                validate_delimiter(Changeset &changeset);
  static void                validate_fields_count(Changeset &changeset);
};

#endif
