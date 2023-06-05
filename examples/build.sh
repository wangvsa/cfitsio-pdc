#export LD_LIBRARY_PATH=../install/lib:$LD_LIBRARY_PATH
gcc ./writer.c -o writer.exe -I../install/include -L../install/lib -lcfitsio
gcc ./reader.c -o reader.exe -I../install/include -L../install/lib -lcfitsio
