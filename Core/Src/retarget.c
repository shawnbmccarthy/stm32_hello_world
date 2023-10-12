/*
 * retarget.c
 *
 *  Created on: Jul 19, 2023
 *      Author: nicolas
 */

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <../Inc/retarget.h>
#include <stdint.h>
#include <stdio.h>

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;

void RetargetInit() {
	// gHuart = huart;

	/* Disable I/O buffering for STDOUT stream, so that
	 * chars are sent out as soon as they are printed. */
	setvbuf(stdout, NULL, _IONBF, 0);
}



int _write(int fd, char *ptr, int len) {
	 HAL_StatusTypeDef hstatus;

	 if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
	 	for (int i = 0; i < len; ++i) {
	 		ITM_SendChar(*ptr++);
	 	}
	 	hstatus = HAL_OK;
	 	if (hstatus == HAL_OK)
	 		return len;
	 	else
	 		return EIO;
	 }
	 errno = EBADF;
	return -1;
}



int _read(int fd, char *ptr, int len) {
	HAL_StatusTypeDef hstatus;

	if (fd == STDIN_FILENO) {
		hstatus = HAL_ERROR;
		if (hstatus == HAL_OK)
			return 1;
		else
			return EIO;
	}
	errno = EBADF;
	return -1;
}


#endif //#if !defined(OS_USE_SEMIHOSTING)
