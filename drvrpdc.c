/*  This file, drvrpdc.c contains driver routines for pdc.         */

/*  The FITSIO software was written by William Pence at the High Energy    */
/*  Astrophysic Science Archive Research Center (HEASARC) at the NASA      */
/*  Goddard Space Flight Center.                                           */

#include <string.h>
#include <stdlib.h>
#include "fitsio2.h"
#include <stdarg.h>
#include "pdc.h"

/*
 * Implementation of the Linux kernel's scnprintf() function.
 * It's snprintf() but returns the number of chars actually written into buf[]
 * not including the '\0'.  It also avoids the -Wformat-truncation warnings.
 */
int scnprintf(char* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    int rc;

    va_start(args, fmt);
    rc = vsnprintf(buf, size, fmt, args);
    va_end(args);

    if (rc >= size) {
        /* We truncated */
        return size - 1;
    }

    return rc;
}

#define LOGDBG(...)                                 \
    do {                                            \
        char msg[1024] = {0};                       \
        scnprintf(msg, sizeof(msg), __VA_ARGS__);   \
        printf("[PDC Driver] %s, %d: %s\n", __func__, __LINE__, msg);   \
    } while(0);


static pdcid_t pdc_id;
static pdcid_t cont_prop;
static pdcid_t cont_id;
static int current_handle;
static bool mpi_init_by_pdc;

typedef struct fits_pdc_obj {
    char*    obj_name;
    pdcid_t  obj_id;
    pdcid_t  obj_prop;
    uint64_t offset;
} fits_pdc_obj_t;

fits_pdc_obj_t handle_table[NMAXFILES];


void _pdc_client_init()
{
    int flag;
    MPI_Initialized(&flag);
    if (!flag) {
       MPI_Init(0, NULL);
       mpi_init_by_pdc = true;
    } else {
       mpi_init_by_pdc = false;
    }

    pdc_id    = PDCinit("pdc");
    cont_prop = PDCprop_create(PDC_CONT_CREATE, pdc_id);
    cont_id   = PDCcont_create_col("c1", cont_prop);
}

void _pdc_client_fini()
{
    PDCcont_close(cont_id);
    PDCprop_close(cont_prop);
    PDCclose(pdc_id);

    if (mpi_init_by_pdc) {
        MPI_Finalize();
    }
}

/* shutdown function is never called by
 * cfistio library, maybe a bug in their
 * library. so we connect/close pdc server
 * connection at open(create) and close time.
 */
int fits_pdc_init(void)
{
    LOGDBG("init, max files: %d", NMAXFILES);

    for(int i = 0; i < NMAXFILES; i++) {
        // initialize all empty slots in table
        handle_table[i].obj_name         = NULL;
        handle_table[i].obj_id           = -1;
        handle_table[i].obj_prop         = -1;
        handle_table[i].offset           = 0;
    }

    _pdc_client_init();
    return 0;
}

int fits_pdc_setoptions(int options)
{
    LOGDBG("setoptions");
    options = 0;
    return 0;
}

int fits_pdc_getoptions(int *options)
{
    LOGDBG("getoptions");
    *options = 0;
    return 0;
}
int fits_pdc_getversion(int *version)
{
    LOGDBG("getversion");
    *version = 99;
    return 0;
}

int fits_pdc_shutdown(void)
{
    LOGDBG("shutdown");
    for (int i = 0; i < NMAXFILES; i++) {
        fits_pdc_close(i);
        fits_pdc_obj_t* obj = &(handle_table[i]);
        if (obj->obj_name)
            free(obj->obj_name);
    }

    return 0;
}

int fits_pdc_truncate(int handle, LONGLONG filesize)
{
    LOGDBG("truncate");
    return 0;
}

int fits_pdc_remove(char *filename)
{
    LOGDBG("remove");
    return 0;
}

int fits_pdc_checkfile (char *urltype, char *infile, char *outfile)
{
    LOGDBG("checkfile %s", urltype);
    return 0;
}

/*
 * open an existing file
 */
int fits_pdc_open(char *filename, int rwmode, int *handle)
{
    LOGDBG("open %s, current_handle: %d", filename, current_handle);
    *handle = -1;
    char *obj_name = filename;
    for (int i = 0; i < current_handle; i++) {
        if (handle_table[i].obj_name != NULL) {
            if (strcmp(obj_name, handle_table[i].obj_name) == 0) {
                LOGDBG("found existing object");
                // found an existing object
                *handle = i;
                break;
            }
        }
    }

    pdcid_t obj_id = PDCobj_open(obj_name, pdc_id);
    if (obj_id == 0) {
        LOGDBG("error when open object %s", obj_name);
        return -1;
    }

    if (*handle == -1) {
        // object previously created by others
        *handle = current_handle;
        current_handle++;
        LOGDBG("object %s not found in handle table, handle: %d", obj_name, *handle);

        handle_table[*handle].obj_name = strdup(obj_name);
        handle_table[*handle].obj_id   = obj_id;
    } else {
        // object previously created by me
        LOGDBG("object %s found in handle table, handle: %d", obj_name, *handle);
        handle_table[*handle].obj_id = obj_id;
    }

    return 0;
}

/*
 * creat a new file
 */
int fits_pdc_create(char *filename, int *handle)
{
    LOGDBG("create %s", filename);

    if (current_handle < NMAXFILES) {
        *handle = current_handle;
        current_handle++;

        pdcid_t obj_id;
        pdcid_t obj_prop;

        // TODO get real dim
        uint64_t dims[1] = {10*1024*1024};

        // First create the object property, these two
        // properties must be set before we can transfer
        obj_prop = PDCprop_create(PDC_OBJ_CREATE, pdc_id);
        PDCprop_set_obj_type(obj_prop, PDC_CHAR);
        PDCprop_set_obj_dims(obj_prop, 1, dims);

        /* 
         * TODO:
         * We can retrive header info and setup more obj prop
         *
         uint64_t dims[NUM_DIMS] = {g_x_ept, g_y_ept};
         PDCprop_set_obj_time_step(*obj_prop, 0);
         PDCprop_set_obj_user_id(*obj_prop, getuid());
         PDCprop_set_obj_app_name(*obj_prop, "cfitsio");
         PDCprop_set_obj_tags(*obj_prop, "tag0=1");
         */

        // Then create the object
        char* obj_name = filename;
        obj_id = PDCobj_create_mpi(cont_id, obj_name, obj_prop, 0, MPI_COMM_WORLD);
        if (obj_id == 0) {
            LOGDBG("error creating an object id of %s from server.", obj_name);
            exit(-1);
        }

        handle_table[*handle].obj_name = strdup(obj_name);
        handle_table[*handle].obj_id   = obj_id;
        handle_table[*handle].obj_prop = obj_prop;
        return 0;
    }

    // exceeds max number of files
    return -1;
}

int fits_pdc_size(int handle, LONGLONG *filesize)
{
    LOGDBG("size");
    *filesize = LONG_MAX;
    return 0 ;
}

int fits_pdc_close(int handle)
{
    LOGDBG("close %d", handle);
    if (handle >= 0 && handle < NMAXFILES) {
        fits_pdc_obj_t* obj = &(handle_table[handle]);
        if (obj->obj_prop != -1) {
            PDCprop_close(obj->obj_prop);
            obj->obj_prop = -1;
        }
        if (obj->obj_id != -1) {
            PDCobj_close(obj->obj_id);
            obj->obj_id = -1;
        }
        // obj->obj_name is still kept in case
        // we need to open the object later
    } else {
        // TODO
        // print error message
    }

    _pdc_client_fini();

    return 0;
}

int fits_pdc_flush(int handle)
{
    LOGDBG("flush %d", handle);
    // TODO
    // do nothing here for now
    return 0;
}

int fits_pdc_seek(int handle, LONGLONG offset)
{
    LOGDBG("seek %d, %ld", handle, offset);
    //handle_table[handle].offset = offset;
    return 0;
}

int fits_pdc_read(int handle, void *buffer, long nbytes)
{
    LOGDBG("read handle: %d, %ld bytes", handle, nbytes);

    pdcid_t local_region, global_region;
    pdcid_t transfer_id;
    fits_pdc_obj_t* obj = &(handle_table[handle]);

    uint64_t local_offsets[1]  = {0};
    uint64_t global_offsets[1] = {obj->offset};
    uint64_t lengths[1]        = {(uint64_t) nbytes};

    struct pdc_obj_info* info = PDCobj_get_info(obj->obj_id);
    LOGDBG("obj id: %d, ndim: %d, local: %ld, global: %ld", obj->obj_id, info->obj_pt->ndim, 0, obj->offset);


    void* data = malloc(sizeof(int) *  nbytes);

    local_region  = PDCregion_create(1, local_offsets,  lengths);
    global_region = PDCregion_create(1, global_offsets, lengths);
    transfer_id   = PDCregion_transfer_create(data, PDC_READ, obj->obj_id,
                                            local_region, global_region);
    PDCregion_transfer_start(transfer_id);
    PDCregion_transfer_wait(transfer_id);

    memcpy(buffer, data, nbytes);
    obj->offset += nbytes;

    return 0;
}

int fits_pdc_write(int handle, void *buffer, long nbytes)
{
    LOGDBG("write handle: %d, %ld bytes, buffer=null?%d", handle, nbytes, buffer==NULL);

    pdcid_t local_region, global_region;
    pdcid_t transfer_id;
    fits_pdc_obj_t* obj = &(handle_table[handle]);

    uint64_t local_offsets[1]  = {0};
    uint64_t global_offsets[1] = {obj->offset};
    uint64_t lengths[1]        = {(uint64_t) nbytes};

    local_region  = PDCregion_create(1, local_offsets,  lengths);
    global_region = PDCregion_create(1, global_offsets, lengths);

    void* data = malloc(sizeof(char) * nbytes);
    memcpy(data, buffer, nbytes);
    transfer_id   = PDCregion_transfer_create(data, PDC_WRITE, obj->obj_id,
                                                local_region, global_region);
    PDCregion_transfer_start(transfer_id);

    // TODO:
    // possible optmization: move the wait call to flush
    // time. need to add a ongoing transfers field.
    // also need to make sure all transfers finished at
    // close time? consistency?
    PDCregion_transfer_wait(transfer_id);

    obj->offset += nbytes;

    return 0;
}
