#ifndef  _VCNPRINTF_HPP
#define  _VCNPRINTF_HPP

#include <stdarg.h>
#include <string.h>
#include <limits.h>

/*
 ** This vsnprintf() emulation does not implement the conversions:
 **	%e, %E, %g, %G, %wc, %ws
 ** The %f implementation is limited.
 */
int vsnprintf(char *buffer, size_t n, const char *format, va_list ap);

#endif     /* -----  not _VCNPRINTF_HPP  ----- */

