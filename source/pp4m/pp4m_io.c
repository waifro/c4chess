/* Private Project Four Me */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

// local headers
#include "pp4m_io.h"

bool pp4m_IO_CheckFile(char *filename) {

    bool check_file = false;
    struct stat st = {0};

    // if file doesn't exist, return -1
    if (stat(filename, &st) != -1) check_file = true;

    return check_file;
}

void pp4m_IO_Feedback(char *filename, const char *text) {

    static bool check_file = false;
    bool result = pp4m_IO_CheckFile(filename);

    // if file existed previously, remove it and set state for runtime
    if (result == true && check_file == false)
    {
        remove(filename);
        check_file = true;
    }

    // if file didn't exist before, set state for runtime
    else if (result == false)
        check_file = true;

    FILE *td = NULL;
    td = fopen(filename, "a");

    fprintf(td, "%s\n", text);

    fclose(td);

    return;

}

/* 2022 @waifro */
