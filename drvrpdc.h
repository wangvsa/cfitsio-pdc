/*  This file, drvrpdc.c contains driver routines for pdc.         */

/*  The FITSIO software was written by William Pence at the High Energy    */
/*  Astrophysic Science Archive Research Center (HEASARC) at the NASA      */
/*  Goddard Space Flight Center.                                           */


int fits_pdc_init(void);

int fits_pdc_setoptions(int options);

int fits_pdc_getoptions(int *options);

int fits_pdc_getversion(int *version);

int fits_pdc_shutdown(void);

int fits_pdc_truncate(int handle, LONGLONG filesize);

int fits_pdc_remove(char *filename);

int fits_pdc_checkfile (char *urltype, char *infile, char *outfile);


/*
 * open an existing file
 */
int fits_pdc_open(char *filename, int rwmode, int *handle);

/*
 * creat a new file
 */
int fits_pdc_create(char *filename, int *handle);

/*
 * return the size of the file in bytes
 */
int fits_pdc_size(int handle, LONGLONG *filesize);

/*
 * don't have to close stdin or stdout 
 */
int fits_pdc_close(int handle);

/*
 * flush the file
 */
int fits_pdc_flush(int handle);

/* 
 * seeking is not supported in pdc
 */
int fits_pdc_seek(int handle, LONGLONG offset);

/*
 * reading from stdin stream 
 */
int fits_pdc_read(int hdl, void *buffer, long nbytes);

/*
 * write bytes at the current position in the file
 */
int fits_pdc_write(int hdl, void *buffer, long nbytes);

