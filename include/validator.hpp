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
