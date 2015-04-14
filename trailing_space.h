#ifndef TRAILING_SPACE_H
#define TRAILING_SPACE_H

#include <stdlib.h>

#define WRONG_ARG_NUMBER (EXIT_SUCCESS - 1)
#define UNKNOWN_FLAG     (EXIT_SUCCESS - 2)

typedef int bool;
#define TRUE  1
#define FALSE 0

#define TEMP_NAME ".tempfilename~"

#define NO_ERR                 0
#define COULD_NOT_OPEN_FILE   -1
#define COULD_NOT_DELETE_FILE -2
#define COULD_NOT_RENAME_FILE -3

#endif
