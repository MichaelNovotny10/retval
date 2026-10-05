CC  = gcc
CMD = echo hi

.PHONY: run clean install uninstall

build: retval.c
	$(CC) retval.c -o retval

run: build
	echo "$(CMD)" | ./retval

clean:
	rm -f ./retval

install: build
	sudo cp ./retval /usr/local/bin/retval

uninstall:
	sudo rm -f /usr/local/bin/retval