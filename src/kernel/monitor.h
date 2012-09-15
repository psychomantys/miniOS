/*
 * =====================================================================================
 *
 *       Filename:  monitor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 08:48:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef MONITOR_H
#define MONITOR_H


// Write a single character out to the screen.
void monitor_put(const char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(const char *c);

#endif // MONITOR_H

