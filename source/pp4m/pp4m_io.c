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

/*                                         Copyright (c) 2021 @richardwaifro //
#                                                                                                                                                    #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), #
# to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, #
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:         #
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.                     #
#                                                                                                                                                    #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,                                                                                    #
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                                #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.                                                                                              #
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,                                                                        #
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                                     #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                            #
#                                                                                                                                                    #
/#                                         Copyright (c) 2021 @richardwaifro */
