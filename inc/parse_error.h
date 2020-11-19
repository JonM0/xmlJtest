#ifndef XML_PARSE_ERROR
#define XML_PARSE_ERROR

/* error handling */

typedef struct _parse_error parse_error;
struct _parse_error
{
    unsigned int code;
    char *message;
};

#define PARSE_ERROR_NONE 0

parse_error *parse_error_create();

void parse_error_add_error(
    parse_error **error,
    unsigned int new_code,
    const char *message);

void parse_error_merge_error(
    parse_error **error,
    parse_error *other);

void parse_error_free(
    const parse_error *error);

#endif