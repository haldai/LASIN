# filename: .gdbinit
# gdb will read it when starting
set solib-search-path /home/daiwz/.local/lib/
set environment LD_LIBRARY_PATH = $(LD_LIBRARY_PATH):/home/daiwz/.local/lib/
