#include <stdio.h>
#include "trailing_space.h"
#include <stdlib.h>

static bool is_whitespace(char c);
static bool has_trailing(FILE *file);
static int remove_trailing(FILE *file);

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

        if ((file = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "Could not open file %s\n", argv[i]);
            continue;
        }

        if (should_remove) {
            if (remove_trailing(file) != 0) {
                fprintf(stderr, "Could not create temp file\n");
            } else {
                fclose(file);
                /* Delete old file. */
                if (remove(argv[i]) != 0) {
                    fprintf(stderr, "Failed to remove old file\n");
                } else {
                    if (rename(TEMP_NAME, argv[i]) != 0) {
                        fprintf(stderr, "Failed to rename file\n");
                    }
                }
            }
        } else {
            printf(has_trailing(file) ? "Yes\n" : "No\n");
            fclose(file);
        }
    }

    return EXIT_SUCCESS;
}

/* Read the file char by char checking if there is ever a whitespace character
 * before an endline character.  Return TRUE if a trailing space is found and
 * FALSE otherwise. */
static bool has_trailing(FILE *file)
{
    char cur_c, prev_c;
    bool has_trailing = FALSE;

    prev_c = cur_c = 'a';

    while ((cur_c = fgetc(file)) != EOF) {
        if (cur_c == '\n' && is_whitespace(prev_c)) {
            has_trailing = TRUE;
            break;
        }

        prev_c = cur_c;
    }

    return has_trailing;
}

/* Create a file with the name TEMP_FILE, and write the content of file to this
 * new file, but without trailing spaces. */
static int remove_trailing(FILE *file)
{
    int spaces = 0;
    char next;
    FILE *out_file;

    if ((out_file = fopen(TEMP_NAME, "w")) == NULL) {
        fprintf(stderr, "Failed to create temp file");
        return 1;
    }

    while ((next = fgetc(file)) != EOF) {
        if (is_whitespace(next)) {
            spaces++;
        } else if(next == '\n') {
            spaces = 0;
            fputc(next, out_file);
        } else {
            while (spaces > 0) {
                fputc(' ', out_file);
                spaces -= 1;
            }

            fputc(next, out_file);
        }
    }

    fclose(out_file);

    return 0;
}

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
