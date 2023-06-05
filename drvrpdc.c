/*  This file, drvrpdc.c contains driver routines for pdc.         */

/*  The FITSIO software was written by William Pence at the High Energy    */
/*  Astrophysic Science Archive Research Center (HEASARC) at the NASA      */
/*  Goddard Space Flight Center.                                           */

#include <string.h>
#include <stdlib.h>
#include "fitsio2.h"


int fits_pdc_init(void)
{
    for(int i = 0; i < NMAXFILES; i++) /* initialize all empty slots in table */
    {
        //handleTable[ii].fileptr = 0;
    }
    return 0;
}

int fits_pdc_setoptions(int options)
{
    options = 0;
    return 0;
}

int fits_pdc_getoptions(int *options)
{
    *options = 0;
    return 0;
}
int fits_pdc_getversion(int *version)
{
    *version = 99;
    return 0;
}

int fits_pdc_shutdown(void)
{
    return 0;
}

int fits_pdc_truncate(int handle, LONGLONG filesize)
{
    return 0;
}

int fits_pdc_remove(char *filename)
{
    return 0;
}

int fits_pdc_checkfile (char *urltype, char *infile, char *outfile)
{
    return 0;
}

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
