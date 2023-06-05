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

int fits_pdc_open(char *filename, int rwmode, int *handle)
{
    // Open existing file
    if (filename)
      rwmode = 1;  /* dummy statement to suppress unused parameter compiler warning */

    *handle = 1;     /*  1 = stdin */

    return(0);
}

int fits_pdc_create(char *filename, int *handle)
{
    // Creat a new file
    if (filename)  /* dummy statement to suppress unused parameter compiler warning */
       *handle = 2;
    else
       *handle = 2;         /*  2 = stdout */

    return(0);
}
/*
 * return the size of the file in bytes
 */
int fits_pdc_size(int handle, LONGLONG *filesize)
{
    handle = 0;  /* suppress unused parameter compiler warning */

    /* this operation is not supported in a stream; return large value */
    *filesize = LONG_MAX;
    return(0);
}
/*
 * don't have to close stdin or stdout 
 */
int fits_pdc_close(int handle)
{
    handle = 0;  /* suppress unused parameter compiler warning */

    return(0);
}
/*
 * flush the file
 */
int fits_pdc_flush(int handle)
{
    if (handle == 2)
       fflush(stdout);  

    return(0);
}
/* 
 * seeking is not allowed in pdc
 */
int fits_pdc_seek(int handle, LONGLONG offset)
{
    offset = handle;  /* suppress unused parameter compiler warning */
    return(1);
}
/*
 * reading from stdin stream 
 */
int fits_pdc_read(int hdl, void *buffer, long nbytes)
{
    long nread;

    if (hdl != 1)
       return(1);  /* can only read from stdin */

    nread = (long) fread(buffer, 1, nbytes, stdin);

    if (nread != nbytes)
    {
        return(END_OF_FILE);
    }
    return(0);
}
/*
 * write bytes at the current position in the file
 */
int fits_pdc_write(int hdl, void *buffer, long nbytes)
{
    if (hdl != 2)
       return(1);  /* can only write to stdout */

    if((long) fwrite(buffer, 1, nbytes, stdout) != nbytes)
        return(WRITE_ERROR);

    return(0);
}
