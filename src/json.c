#include "../inc/json.h"

token_type get_token(char **input, char **token_value, int *token_length) {
    // reset value length;
    *token_length = 0;
    // skip white spaces and new line
    while (**input && (**input == 0x20 || **input == '\n')) {
        (*input)++;
    }
    // tokenise
    switch (**input) {
    case '{':
        (*input)++;
        return TOKEN_BLOCK_START;
    case '}':
        (*input)++;
        return TOKEN_BLOCK_END;
    case '[':
        (*input)++;
        return TOKEN_ARRAY_START;
    case ']':
        (*input)++;
        return TOKEN_ARRAY_END;
    case ':':
        (*input)++;
        return TOKEN_COLON;
    case ',':
        (*input)++;
        return TOKEN_COMMA;
    case '"':
        (*input)++;
        *token_value = *input;
        while (**input && **input != '"') {
            (*token_length)++;
            (*input)++;
        }
        if (**input == '"') {
            (*input)++;
        }
        return TOKEN_STRING;
    case 't':
        if (strncmp(*input, "true", 4) == 0) {
            (*input) += 4;
            return TOKEN_BOOL_TRUE;
        } else {
            (*input)++;
            return TOKEN_UNKNOWN;
        }
    case 'f':
        if (strncmp(*input, "false", 5) == 0) {
            (*input) += 5;
            return TOKEN_BOOL_FALSE;
        } else {
            (*input)++;
            return TOKEN_UNKNOWN;
        }
    case 'n':
        if (strncmp(*input, "null", 4) == 0) {
            (*input) += 4;
            return TOKEN_NULL;
        } else {
            (*input)++;
            return TOKEN_UNKNOWN;
        }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        *token_value = *input;
        (*token_length)++;
        (*input)++;
        while ((**input <= '9' && **input >= '0') || **input == '.') {
            (*token_length)++;
            (*input)++;
        }
        return TOKEN_NUMBER;
    case '\0':
        return TOKEN_END;
    default:
        (*input)++;
        return TOKEN_UNKNOWN;
    }
}

int parse_json(char **buffer, struct json_object *root) {
    char **input = buffer;
    token_type token;
    char *value;
    int value_length;
    int property_index = 0;
    int property_state = STATE_KEY;

    // main loop
    while ((token = get_token(input, &value, &value_length)) != TOKEN_END) {
        switch (token)
        {
        case TOKEN_BLOCK_START:
            // create new json object
            root->property_list_length = 1;
            root->property_list = (struct json_object *) malloc(sizeof(struct json_object));
            root->value = NULL;
            if (parse_json(input, &(root->property_list[property_index++])) != PARSING_SUCCESS) {
                return(PARSING_FAILURE);
            }
            break;
        case TOKEN_BLOCK_END:
            return(PARSING_SUCCESS);
            break;
        case TOKEN_COLON:
            //TODO: use for test of json structure
            break;
        case TOKEN_COMMA:
            // expand property list of one json object
            root->property_list_length++;
            root->property_list = (struct json_object *) realloc(root->property_list, root->property_list_length * sizeof(struct json_object));
            if (parse_json(input, &(root->property_list[property_index++])) != 0) {
                return(PARSING_FAILURE);
            }
            break;
        case TOKEN_STRING:
            if (property_state == STATE_KEY) {
                // set key
                root->key = (char *) malloc(sizeof(char) * (value_length + 1));
                memcpy(root->key, value, value_length);
                root->key[value_length] = '\0';
                property_state = STATE_VALUE;
            } else {
                // set value
                root->value = (char *) malloc(sizeof(char) * (value_length + 1));
                memcpy(root->value, value, value_length);
                root->value[value_length] = '\0';
                property_state = STATE_KEY;
                return(PARSING_SUCCESS);
            }
            break;
        case TOKEN_NUMBER:
            root->value = (char *) malloc(sizeof(char) * (value_length + 1));
            memcpy(root->value, value, value_length);
            root->value[value_length] = '\0';
            property_state = STATE_KEY;
            return(PARSING_SUCCESS);
            break;
        case TOKEN_BOOL_TRUE:
            printf("Bool: true\n");
            break;
        case TOKEN_BOOL_FALSE:
            printf("Bool: false\n");
            break;
        case TOKEN_NULL:
            printf("Null\n");
            break;
        case TOKEN_UNKNOWN:
            printf("Unknown\n");
            return(PARSING_FAILURE);
            break;
        default:
            break;
        }
    }
    return(0);
}

char *get_value(char *path, struct json_object *root) {
    char **keys = &path;
    return(search_key(keys, root));
}

char *search_key(char **keys, struct json_object *root) {
    char *key = *keys;
    int index;
    while (**keys != '.' && **keys != '\0') {
        (*keys)++;
    }
    if (**keys == '.') {
        **keys = '\0';
        (*keys)++;
        // there is no key to search
        if (root->property_list == NULL) {
            return("NULL");
        }
        for (index = 0; index < root->property_list_length; index++) {
            if (strcmp(key, root->property_list[index].key) == 0) {
                return(search_key(keys, &(root->property_list[index])));
            }
        }
        // key doesnt exists
        return("NULL");
    } else {
        // there is no key to search
        if (root->property_list == NULL) {
            return("NULL");
        }
        for (index = 0; index < root->property_list_length; index++) {
            if (strcmp(key, root->property_list[index].key) == 0) {
                if (root->property_list[index].value == NULL) {
                    return("NULL");
                }
                return(root->property_list[index].value);
            }
        }
        // key doesnt exists
        return("NULL");
    }
}