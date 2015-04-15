#ifndef TRAILING_SPACE_H
#define TRAILING_SPACE_H

#include <stdlib.h>

/* Program error codes. */
#define WRONG_ARG_NUMBER (EXIT_SUCCESS - 1)
#define UNKNOWN_FLAG     (EXIT_SUCCESS - 2)

/* Define boolean values. */
typedef int bool;
#define TRUE  1
#define FALSE 0

/* Function error codes. */
#define NO_ERR                ( 0)
#define COULD_NOT_OPEN_FILE   (-1)
#define COULD_NOT_DELETE_FILE (-2)
#define COULD_NOT_RENAME_FILE (-3)
#define TEMP_FILE_CREATION    (-4)

/* Other definitions. */
#define TEMP_NAME ".tempfilename~"

#endif
