project(rwqueue CXX)
add_library(rwqueue atomicops.h readerwritercircularbuffer.h readerwriterqueue.h)
set_target_properties(rwqueue PROPERTIES LINKER_LANGUAGE CXX)