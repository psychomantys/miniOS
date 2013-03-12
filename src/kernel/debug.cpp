/*
 * =====================================================================================
 *
 *       Filename:  debug.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/2013 08:57:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<kernel/monitor.hpp>
#include	<kernel/debug.hpp>
#include	<stdarg.h>

int kdebug(int debug_level, const char *format, ... ){
#ifdef DEBUG
	unsigned int debug=DEBUG;
#else
	unsigned int debug=0;
#endif
	int rv=0;
	if( debug_level<=debug ){
		va_list ap;
		va_start(ap, format);
		rv = kprintf(format, ap);
//		Deveria funcionar isso:
		va_end(ap);
	}
	return rv;
}

