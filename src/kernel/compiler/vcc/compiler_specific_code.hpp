/*
 * =====================================================================================
 *
 *       Filename:  compiler_specific_code.hpp
 *
 *    Description:  Codigo especifico para um compilador.
 *
 *        Version:  1.0
 *        Created:  09/18/2012 12:38:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */
#ifndef	_COMPILER_SPECIFIC_CODE_HPP__INC
#define	_COMPILER_SPECIFIC_CODE_HPP__INC

void	*__dso_handle=0;

extern "C" void _at_global_begin();

extern "C" void _at_global_end();

#endif   /* ----- #ifndef _COMPILER_SPECIFIC_CODE_HPP__INC  ----- */

