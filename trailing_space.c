#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "trailing_space.h"

static bool is_whitespace(char c);
static int has_trailing(char const *filename);
static int remove_trailing(char const *filename);
static char * get_temp_file_name(void);

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
            case TEMP_FILE_CREATION:
                fprintf(stderr, "Temp creation error: file %s\n", argv[i]);
            default:
                fprintf(stderr, "Unknown error: file with %s\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}

/* Read the file given as filename and write to stdout if the file has trailing
 * whitespace or not. */
static int has_trailing(char const *filename)
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

/* Change the content of the file given by filename to not include trailing
 * spaces.  The function will collapse all tabs characters in the file to a
 * space. */
static int remove_trailing(char const *filename)
{
    int spaces = 0;
    char next;
    FILE *in_file;
    FILE *out_file;
    char *temp_filename;

    if ((temp_filename = get_temp_file_name()) == NULL)
        return TEMP_FILE_CREATION;

    if ((in_file = fopen(filename, "r")) == NULL)
        return COULD_NOT_OPEN_FILE;

    if ((out_file = fopen(temp_filename, "w")) == NULL)
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

/* The function returns TRUE if the character c is a space or tab character and
 * false otherwise. */
static bool is_whitespace(char c)
{
    return (c == ' ' || c == '\t');
}

/* The function returns a not used filename that can be used to generate a
 * temporary file.  The function returns a NULL pointer if no unique filename
 * can be found. */
static char * get_temp_file_name(void) {
    int max_filename_size = 128;
    char *temp_file_name = malloc(sizeof(char) * max_filename_size);
    int filename_size;

    assert(temp_file_name != NULL);

    strcpy(temp_file_name, TEMP_NAME);
    /*temp_file_name = TEMP_NAME;*/
    filename_size = strlen(temp_file_name);

    /* Change the filename while the file exist to get a unique name. */
    while (filename_size + 1 != max_filename_size) {
        if (access(temp_file_name, F_OK) == -1) {
            return temp_file_name;
        }

        temp_file_name[filename_size++] = '~';
        temp_file_name[filename_size] = '\0';
    }

    return NULL;
}
