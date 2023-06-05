/*  This file, drvrpdc.c contains driver routines for pdc.         */

/*  The FITSIO software was written by William Pence at the High Energy    */
/*  Astrophysic Science Archive Research Center (HEASARC) at the NASA      */
/*  Goddard Space Flight Center.                                           */

#include <string.h>
#include <stdlib.h>
#include "fitsio2.h"

/**********************************************************************/
/*******          driver routines for pdc//:                   ********/
/**********************************************************************/

/*
 * open an existing file
 */
int fits_pdc_open(char *filename, int rwmode, int *handle)
{
    *handle = 1;    // pdc it to fits handle
    return(0);
}

/*
 * creat a new file
 */
int fits_pdc_create(char *filename, int *handle)
{
    *handle = 1;    // pdc it to fits handle
    return 0;
}
/*
 * return the size of the file in bytes
 */
int fits_pdc_size(int handle, LONGLONG *filesize)
{
    *filesize = LONG_MAX;
    return 0 ;
}
/*
 * don't have to close stdin or stdout 
 */
int fits_pdc_close(int handle)
{
    return 0;
}
/*
 * flush the file
 */
int fits_pdc_flush(int handle)
{
    return 0;
}
/* 
 * seeking is not supported in pdc
 */
int fits_pdc_seek(int handle, LONGLONG offset)
{
    return 0;
}
/*
 * reading from stdin stream 
 */
int fits_pdc_read(int hdl, void *buffer, long nbytes)
{
    return 0;
}
/*
 * write bytes at the current position in the file
 */
int fits_pdc_write(int hdl, void *buffer, long nbytes)
{
    return 0;
}
