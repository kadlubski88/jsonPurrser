#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define PERROR fprintf(stderr, "Error in file %s at line %d: %s\n", __FILE__, __LINE__, strerror(errno))
#define BUFFER_SIZE 4096

#define PARSING_SUCCESS 0
#define PARSING_FAILURE 1

typedef enum{
    TOKEN_BLOCK_START,
    TOKEN_BLOCK_END,
    TOKEN_ARRAY_START,
    TOKEN_ARRAY_END,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_BOOL_TRUE,
    TOKEN_BOOL_FALSE,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_NULL,
    TOKEN_END,
    TOKEN_UNKNOWN
}token_type;

typedef enum{
    STATE_KEY,
    STATE_VALUE,
}key_value_state;

struct json_object{
    char *key;
    char *value;
    struct json_object *property_list;
    int property_list_length;
};


token_type get_token(char **input, char **token_value, int *token_length);
int parse_json(char **buffer, struct json_object *root);
char *get_value(char *path, struct json_object *root);
char *search_key(char **keys, struct json_object *root);

#endif // _HEADER_H_