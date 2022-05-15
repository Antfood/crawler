#ifndef PARSER_H
#define PARSER_H
#include "../include/doctest.h"
#include <regex>
#include <array>
#include <iostream>
#include <vector>

#define REGEX_LEN 10
#define DOT "."
#define DELIM "_"
#define FIELD_COUNT 11 /* 10 fields + extention */

enum field_type 
{
  client           = 0,
  project          = 1,
  name             = 2,
  descritor        = 3,
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
    std::vector<std::string> m_fields;
    std::vector<ValidationError>   m_errors;
    bool                           m_valid;

    Changeset(std::vector<std::string> &&fields);

    const std::string             error_to_string(const int pos);
    void                          invalidate(const error_type err, int pos);

};

class Validator
{
  std::string                      m_delimiter;
  size_t                           m_cursor;
  struct                           Private;

  public:
  Validator(const std::string &delimeter);

  Changeset                  split(const std::string &filename);
  void                       validate_delimiter(Changeset &changeset);
  void                       validate_fields_count(Changeset &changeset);
  void                       validate_fields(Changeset &changeset);
  static std::string         type_to_string(field_type type);
};

#endif
