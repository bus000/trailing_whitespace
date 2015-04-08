#include <stdio.h>
#include "trailing_space.h"
#include <stdlib.h>

static bool is_whitespace(char c);

int main(int argc, char const *argv[])
{
    FILE *file;
    char cur_c, prev_c;
    bool has_whitespace = FALSE;

    /* Test the number of arguments. */
    if (argc != 2) {
        fprintf(stderr, "Program expects two arguments.\n");
        return WRONG_ARG_NUMBER;
    }

    /* Open file with read rights. */
    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return FILE_OPEN_ERROR;
    }

    /* Read the file char by char checking if there is ever a whitespace
     * character before an endline character. */
    prev_c = cur_c = 'a';
    while ((cur_c = fgetc(file)) != EOF) {
        if (cur_c == '\n' && is_whitespace(prev_c)) {
            has_whitespace = TRUE;
            break;
        }

        prev_c = cur_c;
    }

    printf(has_whitespace ? "Yes\n" : "No\n");

    return EXIT_SUCCESS;
}

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
