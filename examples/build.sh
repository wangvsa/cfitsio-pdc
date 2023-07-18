#export LD_LIBRARY_PATH=/g/g90/wang116/sources/cfitsio-pdc/install/lib:$LD_LIBRARY_PATH

export CFITSIO_DIR=/g/g90/wang116/sources/cfitsio-pdc/install
mpicc -g ./writer.c -o writer.exe -I$CFITSIO_DIR/include -L$CFITSIO_DIR/lib -lcfitsio
mpicc -g ./reader.c -o reader.exe -I$CFITSIO_DIR/include -L$CFITSIO_DIR/lib -lcfitsio
