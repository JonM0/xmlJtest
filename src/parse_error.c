#include "parse_error.h"

#include <stdlib.h>
#include <string.h>

/* error handling functions */

parse_error *parse_error_create()
{
    parse_error *error = NULL;

    error = (parse_error *)malloc(sizeof(parse_error));

    if (error)
    {
        error->code = PARSE_ERROR_NONE;
        error->message = NULL;
    }

    return error;
}

void parse_error_add_error(parse_error **error, unsigned int new_code, const char *message)
{
    if (*error == NULL)
    {
        *error = parse_error_create();
    }

    if (*error)
    {
        /* OR the code */
        (*error)->code |= new_code;

        /* append eventual message */
        if (message)
        {
            /* there was already a message */
            if ((*error)->message)
            {
                (*error)->message = realloc((*error)->message, 
                    sizeof(char) * 
                        (strlen((*error)->message) + strlen(message) + 1));
                if ((*error)->message)
                {
                    strcat((*error)->message, message);
                }
            }
            else
            {
                (*error)->message = strdup(message);
            }
        }
    }
}

void parse_error_merge_error(parse_error **error, parse_error *other)
{
    if (other)
    {
        if (*error)
        {
            parse_error_add_error(error, other->code, other->message);
            parse_error_free(other);
        }
        else
        {
            *error = other;
        }
    }
}

void parse_error_free(const parse_error *error)
{
    if (error)
    {
        free(error->message);
        free((void *)error);
    }
}