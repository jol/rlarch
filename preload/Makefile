# Note you have to define the RLARCH_PREFIX env variable.

RLACFLAGS=-D_GNU_SOURCE -Wall -fPIC -shared
RLALDFLAGS=-L${RLARCH_PREFIX}/usr/lib/ -ldl -lc -Wl,-rpath=${RLARCH_PREFIX}/usr/lib

all: librlarch.so

librlarch.so: rlarch.c
	gcc ${RLACFLAGS} -o librlarch.so rlarch.c ${RLALDFLAGS} 

clean:
	rm *.so *~

