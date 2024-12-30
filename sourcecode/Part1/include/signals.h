#ifndef SIGNALS_H
#define SIGNALS_H

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

/**
 * @author Marco
 * @brief Claims all zombie processes
 */
void ClaimChildren();

/**
 * @author Marco
 * @brief Defines handler behaviour for different signals
 * @param sig_num The signal type(s)
 */
void HandleSigs(int sig_num);

/**
 * @author Marco
 * @brief Sets up signals to be blocked and signal handlers by adding them to a set of Signals.
 */
int SetupSigs();

/**
 * @author Marco
 * @brief Toggles Blocking on/off for desired signal(s)
 * @param how The sigproc how value. Can be SIG_SETMASK, SIG_BLOCK or SIG_UNBLOCK
 * @param sig_numb The current signal set 
 */
int SigBlock(int how, int sig_num);

#endif