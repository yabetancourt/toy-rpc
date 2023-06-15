CC := gcc

PACKAGES := \
	drmollineda.so \
	yabetancourt.so

%.so: proc_%.c
	$(CC) -shared -fpic -o $@ $^ -lm

all: ${PACKAGES}

clean:
	rm -rf *.o *.so
