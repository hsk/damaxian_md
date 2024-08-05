all:
	docker run --rm -v "`pwd`":/src sgdk
	picodrive out/rom.bin 2> /dev/null
r:
	cd res; python bg.py > ../src/bg.c
clean:
	rm -rf out
