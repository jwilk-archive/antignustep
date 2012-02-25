/*
 * Copyright © 2011 Jakub Wilk <jwilk@jwilk.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dlfcn.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int done;
static int (*original_mkdir)(const char *, mode_t);

int mkdir(const char *path, mode_t mode)
{
    if (mode != 0777) {
        done = 1;
        return original_mkdir(path, mode);
    }
    if (!done) {
        done = 1;
        /* Don't allow creating the very first directory (which should be
         * $HOME/GNUstep), but only if the call passes sanity checks.
         */
        if (mode == 0777) {
            struct passwd* pw = getpwuid(getuid());
            if (pw == NULL || pw->pw_dir)
                return -1;
            size_t home_len = strlen(pw->pw_dir);
            if (strncmp(path, pw->pw_dir, home_len) == 0 && strcmp(path + home_len, "/GNUstep") == 0) {
                errno = EACCES;
                return -1;
            }
        }
    }
    if (!original_mkdir) {
        original_mkdir = dlsym(RTLD_NEXT, "mkdir");
        if (!original_mkdir) {
            errno = EINVAL;
            return -1;
        }
    }
    return original_mkdir(path, mode);
}

/* vim:set ts=4 sw=4 et:*/
