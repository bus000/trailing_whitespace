#include <stdio.h>
#include "trailing_space.h"
#include <stdlib.h>

static bool is_whitespace(char c);
static bool has_trailing(FILE *file);

int main(int argc, char const *argv[])
{
    int i;
    FILE *file;

    /* Test the number of arguments. */
    if (argc < 2) {
        fprintf(stderr, "Program expects at least 1 argument.\n");
        return WRONG_ARG_NUMBER;
    }

    for (i = 1; i < argc; i++) {
        /* Open file with read rights. */
        if ((file = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "Could not open file %s\n", argv[i]);
        }

        printf(has_trailing(file) ? "Yes\n" : "No\n");

        /* Close file. */
        fclose(file);
    }

    return EXIT_SUCCESS;
}

/* Read the file char by char checking if there is ever a whitespace character
 * before an endline character.  Return TRUE if a trailing space is found and
 * FALSE otherwise. */
static bool has_trailing(FILE *file)
{
    char cur_c, prev_c; prev_c = cur_c = 'a';
    bool has_trailing = FALSE;

    while ((cur_c = fgetc(file)) != EOF) {
        if (cur_c == '\n' && is_whitespace(prev_c)) {
            has_trailing = TRUE;
            break;
        }

        prev_c = cur_c;
    }

    return has_trailing;
}

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
