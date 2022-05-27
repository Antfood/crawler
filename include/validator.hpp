#ifndef PARSER_H
#define PARSER_H
#include "../include/doctest.h"
#include "../include/changeset.hpp"
#include <regex>
#include <array>
#include <iostream>
#include <vector>
#include <exception>
#include <utility>

#define REGEX_COUNT 9
#define REGEX_FIELD_ERR_FIELD_OFFSET 4
#define TYPE_FIELD_ERR_FIELD_OFFSET 2

enum field_type
{
  client           = 0,
  project          = 1,
  recording        = 2,
  descriptor       = 3,
  date             = 4,
  key              = 5,
  bpm              = 6,
  ts               = 7,
  composer         = 8,
  external_talent  = 9,
  ext              = 10
};

enum regex_type
{
  name_regex       = 0,
  descriptor_regex = 1,
  date_regex       = 2,
  key_regex        = 3,
  bpm_regex        = 4,
  ts_regex         = 5,
  composer_regex   = 6,
  talent_regex     = 7,
  ext_regex        = 8,
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
  static void                fix_fields_index(Changeset &changeset);
};

#endif
