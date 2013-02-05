/*
 * =====================================================================================
 *
 *       Filename:  snprintf.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/25/2013 09:06:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<kernel/kpp/snprintf.hpp>

#ifndef HAVE_SNPRINTF
#define HAVE_SNPRINTF
int snprintf(char *buffer, size_t n, const char *format, ...){
	va_list ap;
	int rv;

	va_start(ap, format);
	rv = vsnprintf(buffer, n, format, ap);
//	Era para ter isso:
	va_end( ap );
	return rv;
}
#endif

