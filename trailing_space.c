#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "trailing_space.h"

static bool is_whitespace(char c);
static int has_trailing(const char *filename);
static int remove_trailing(const char *filename);

int main(int argc, char const *argv[])
{
    int i;
    int ret_val = NO_ERR;
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

        if (should_remove) {
            ret_val = remove_trailing(argv[i]);
        } else {
            ret_val = has_trailing(argv[i]);
        }

       switch (ret_val) {
            case NO_ERR:

                break;
            case COULD_NOT_OPEN_FILE:
                fprintf(stderr, "Open file error: file %s\n", argv[i]);
                break;
            case COULD_NOT_DELETE_FILE:
                fprintf(stderr, "Delete error: file %s\n", argv[i]);
                break;
            default:
                fprintf(stderr, "Unknown error: file with %s\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}

/* Read the file char by char checking if there is ever a whitespace character
 * before an endline character.  Return TRUE if a trailing space is found and
 * FALSE otherwise. */
static int has_trailing(const char *filename)
{
    FILE *file_in;
    char cur_c, prev_c;
    bool has_trailing = FALSE;

    if ((file_in = fopen(filename, "r")) == NULL)
        return COULD_NOT_OPEN_FILE;

    prev_c = cur_c = 'a';

    while ((cur_c = fgetc(file_in)) != EOF) {
        if (cur_c == '\n' && is_whitespace(prev_c)) {
            has_trailing = TRUE;
            break;
        }

        prev_c = cur_c;
    }

    if (has_trailing)
        printf("file %s has trailing\n", filename);
    else
        printf("file %s has no trailing\n", filename);

    return NO_ERR;
}

/* Create a file with the name TEMP_FILE, and write the content of file to this
 * new file, but without trailing spaces. */
static int remove_trailing(const char *filename)
{
    int spaces = 0;
    char next;
    FILE *in_file;
    FILE *out_file;

    if ((in_file = fopen(filename, "r")) == NULL)
        return COULD_NOT_OPEN_FILE;

    if ((out_file = fopen(TEMP_NAME, "w")) == NULL)
        return COULD_NOT_OPEN_FILE;

    while ((next = fgetc(in_file)) != EOF) {
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

    fclose(in_file);
    fclose(out_file);

    /* Remove old file and move new file to its old position. */
    if (unlink(filename) != 0)
        return COULD_NOT_DELETE_FILE;

    if (rename(TEMP_NAME, filename) != 0)
        return COULD_NOT_RENAME_FILE;

    return NO_ERR;
}

static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}
