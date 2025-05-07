#include "../inc/json.h"

int
main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    char buffer[BUFFER_SIZE];
    int count = 0;
    char *input = buffer;

    // read input
    if ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) < 0) {
        PERROR;
    }
    if (count != 0) {
        buffer[count] = '\0';
    } else {
        fprintf(stderr, "Error: No input found.\n");
        exit(EXIT_FAILURE);
    }

    struct json_object *root;
    root = (struct json_object *) malloc(sizeof(struct json_object));
    if (parse_json(&input, root) != PARSING_SUCCESS) {
        exit(EXIT_FAILURE);
    }

    if (argc != 2) {
        fprintf(stderr, "Error: Wrong usage.\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", get_value(argv[1], root));
    

    exit(EXIT_SUCCESS);
}