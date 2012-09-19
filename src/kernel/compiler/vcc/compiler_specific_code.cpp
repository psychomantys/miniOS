/*
 * =====================================================================================
 *
 *       Filename:  compiler_specific_code.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/19/2012 12:24:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include	<compiler_specific_code.hpp>

typedef void (*_PVFV)(void);
typedef int  (*_PIFV)(void);
typedef void (*_PVFI)(int);

#pragma data_seg(".CRT$XIA")
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = {0};
#pragma data_seg(".CRT$XIZ")
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = {0};
#pragma data_seg(".CRT$XCA")
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = {0};
#pragma data_seg(".CRT$XCZ")
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = {0};
#pragma data_seg(".CRT$XPA")
__declspec(allocate(".CRT$XPA")) _PVFV __xp_a[] = {0};
#pragma data_seg(".CRT$XPZ")
__declspec(allocate(".CRT$XPZ")) _PVFV __xp_z[] = {0};
#pragma data_seg(".CRT$XTA")
__declspec(allocate(".CRT$XTA")) _PVFV __xt_a[] = {0};
#pragma data_seg(".CRT$XTZ")
__declspec(allocate(".CRT$XTZ")) _PVFV __xt_z[] = {0};
#pragma data_seg()
#pragma comment(linker, "/merge:.CRT=.data")

static _PVFV onexitarray[32];
static _PVFV *onexitbegin, *onexitend;

int __cdecl _purecall()
{
        // print error message
}

int __cdecl atexit(_PVFV fn)
{
        if (32*4 < ((int)onexitend-(int)onexitbegin)+4)
                return 1;
        else
                *(onexitend++) = fn;
        return 0;
}

EXTERN int runInit()
{
        // init the __xi_a to __xi_z:  __initex(__xi_a, __xi_z);
        // init __xc_a to __xc_z
}

static void __init(_PVFV *pfbegin, _PVFV *pfend)
{
    while (pfbegin < pfend)
    {
        if (*pfbegin != 0)
            (**pfbegin)();
        ++pfbegin;
    }
}

static int __initex(_PIFV *pfbegin, _PIFV *pfend)
{
        int ret = 0;

        while (pfbegin < pfend && ret == 0)
        {
            if (*pfbegin != 0)
                ret = (**pfbegin)();
            ++pfbegin;
        }

        return ret;
}

EXTERN void runUninit()
{
        if (onexitbegin)
        {
                while (--onexitend >= onexitbegin)
                        if (*onexitend != 0)
                                (**onexitend)();
        }

        __init(__xp_a, __xp_z);
        __init(__xt_a, __xt_z);
}

EXTERN int onexitinit()
{
        onexitend = onexitbegin = onexitarray;
        *onexitbegin = 0;
        return 0;
}

#pragma data_seg(".CRT$XIB")      // run onexitinit automatically
__declspec(allocate(".CRT$XIB")) static _PIFV pinit = onexitinit;
#pragma data_seg()

extern "C" void _at_global_begin(){
	runInit();
}

extern "C" void _at_global_end(){
	runTerm();
}

