/*

  Copyright (c) 2015 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#ifndef LIBMILL_H_INCLUDED
#define LIBMILL_H_INCLUDED

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************/
/*  ABI versioning support                                                    */
/******************************************************************************/

/*  Don't change this unless you know exactly what you're doing and have      */
/*  read and understand the following documents:                              */
/*  www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html     */
/*  www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html  */

/*  The current interface version. */
#define MILL_VERSION_CURRENT 8

/*  The latest revision of the current interface. */
#define MILL_VERSION_REVISION 0

/*  How many past interface versions are still supported. */
#define MILL_VERSION_AGE 1

/******************************************************************************/
/*  Symbol visibility                                                         */
/******************************************************************************/

#if defined MILL_NO_EXPORTS
#   define MILL_EXPORT
#else
#   if defined _WIN32
#      if defined MILL_EXPORTS
#          define MILL_EXPORT __declspec(dllexport)
#      else
#          define MILL_EXPORT __declspec(dllimport)
#      endif
#   else
#      if defined __SUNPRO_C
#          define MILL_EXPORT __global
#      elif (defined __GNUC__ && __GNUC__ >= 4) || \
             defined __INTEL_COMPILER || defined __clang__
#          define MILL_EXPORT __attribute__ ((visibility("default")))
#      else
#          define MILL_EXPORT
#      endif
#   endif
#endif

/******************************************************************************/
/*  Helpers                                                                   */
/******************************************************************************/

MILL_EXPORT int64_t now(void);

/******************************************************************************/
/*  Coroutines                                                                */
/******************************************************************************/

MILL_EXPORT int goprepare(int count, size_t stack_size, size_t val_size);

MILL_EXPORT extern volatile int mill_unoptimisable1;
MILL_EXPORT extern volatile void *mill_unoptimisable2;

MILL_EXPORT void *mill_go_prologue(void);
MILL_EXPORT void mill_go_epilogue(void);

MILL_EXPORT void mill_yield(void);
MILL_EXPORT void mill_msleep(int64_t deadline);

#define FDW_IN 1
#define FDW_OUT 2
#define FDW_ERR 4

MILL_EXPORT int mill_fdwait(int fd, int events, int64_t deadline);

MILL_EXPORT void *cls(void);
MILL_EXPORT void setcls(void *val);

/******************************************************************************/
/*  Channels                                                                  */
/******************************************************************************/

typedef struct mill_chan *chan;

#define MILL_CLAUSELEN (sizeof(struct{void *f1; void *f2; void *f3; void *f4; \
    void *f5; void *f6; int f7; int f8; int f9;}))

MILL_EXPORT chan mill_chmake(size_t sz, size_t bufsz);
MILL_EXPORT chan mill_chdup(chan ch);
MILL_EXPORT void mill_chs(chan ch, void *val, size_t sz);
MILL_EXPORT void *mill_chr(chan ch, size_t sz);
MILL_EXPORT void mill_chdone(chan ch, void *val, size_t sz);
MILL_EXPORT void mill_chclose(chan ch);

MILL_EXPORT void mill_choose_init(void);
MILL_EXPORT void mill_choose_in(void *clause, chan ch, size_t sz, int idx);
MILL_EXPORT void mill_choose_out(void *clause, chan ch, void *val, size_t sz, int idx);
MILL_EXPORT void mill_choose_otherwise(void);
MILL_EXPORT int mill_choose_wait(void);
MILL_EXPORT void *mill_choose_val(size_t sz);

MILL_EXPORT void mill_panic(const char *text);

#endif

