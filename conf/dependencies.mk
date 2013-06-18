allegro.c: include/allegro.h
getopt.c: include/getopt.h
list.c: include/utils.h include/list/list-internal.h \
 include/list/list.h include/list/list-item.h include/terrain.h
main.c: include/test.h include/utils.h include/river/river.h \
 include/strip.h include/terrain.h include/getopt.h include/allegro.h
river.c: include/allegro.h include/list/list.h \
 include/list/list-item.h include/terrain.h include/utils.h \
 include/strip.h include/terrain.h include/river/river-internal.h \
 include/river/river.h
statistical.c: include/statistical.h
strip.c: include/strip.h include/terrain.h include/utils.h \
 include/terrain.h include/statistical.h
test.c: include/test.h include/list/list.h \
 include/list/list-item.h include/terrain.h include/strip.h \
 include/terrain.h include/river/river-internal.h include/river/river.h
utils.c: include/utils.h
