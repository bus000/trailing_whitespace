#include <stdio.h>
#include "trailing_space.h"
#include <stdlib.h>

static bool is_whitespace(char c);
static bool has_trailing(FILE *file);
static void remove_trailing(FILE *file);

int main(int argc, char const *argv[])
{
    int i;
    FILE *file;
    bool should_remove = FALSE;

    /* Test the number of arguments. */
    if (argc < 2) {
        fprintf(stderr, "Program expects at least 1 argument.\n");
        return WRONG_ARG_NUMBER;
    }

    /* Look for flags. */
    for (i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            switch (*(argv[i] + 1)) {
                case 'r':
                    should_remove = TRUE;
                    break;
                default:
                    fprintf(stderr, "Unknown argument %s\n", argv[i] + 1);
                    return UNKNOWN_FLAG;
                    break;
            }
        }
    }

    for (i = 1; i < argc; i++) {
        if (*argv[i] == '-') break; /* Break on flag. */

        if ((file = fopen(argv[i], "rw")) == NULL) {
            fprintf(stderr, "Could not open file %s\n", argv[i]);
        }

        if (should_remove) {
            remove_trailing(file);
        } else {
            printf(has_trailing(file) ? "Yes\n" : "No\n");
        }

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

static void remove_trailing(FILE *file)
{
    int size = 4096;
    char *buffer = malloc(sizeof(char) * size);
    char *end = buffer + size - 1;
    char *next = buffer;
    char cur;

    while ((cur = fgetc(file)) != EOF) {
        if (cur == '\n') {
            while (is_whitespace(*(next - 1)) && (next - 1) != buffer)
                next--;
            *++next = '\n';
        } else {
            if (next == end) { /* Reallocate. */

            } else {
                *next++ = cur;
            }
        }
    }
    *next = EOF;

    fprintf(file, "%s", buffer);
}

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
