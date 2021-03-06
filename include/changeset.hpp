#ifndef CHANGESET_H
#define CHANGESET_H
#include <vector>
#include <string>
#include <regex>
#include <array>

#define DELIM "_"
#define FIELD_COUNT 11  /* 10 fields + extension */
#define ERROR_TYPE_FIELD_TYPE_OFFSET 2

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

using ValidationError = std::pair <error_type, size_t>;

struct Changeset
{
 public:
  std::vector<std::string>       m_fields;
  std::vector<ValidationError>   m_errors;
  bool                           m_valid;
  bool                           m_cleared;
  bool                           m_quit;
  bool                           m_skipped;

  explicit Changeset(std::vector<std::string> &&fields);
  struct                  Private;
  std::string             other_errors();
  std::string             first_error();
  void                    fill_empty_fields();
  void                    invalidate(error_type err, int pos);
  bool                    has_error(error_type err);
  void                    clear_errors();
  void                    clear_error(error_type err);
  std::string             build_path();
  bool                    error_is_bad_field_count();

  static void            fill_fields(std::vector<std::string> &fields);
};

#endif
