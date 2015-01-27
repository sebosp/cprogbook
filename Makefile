CFLAGS=-Wall -g
export PATH := /usr/lib/colorgcc/bin/:$(PATH)
export CCACHE_PATH := /usr/bin
CC=/usr/bin/colorgcc

clean:
	find . -maxdepth 1 -executable -type f -name '*ex[0-9]-[0-9]*' -exec rm {} \;
