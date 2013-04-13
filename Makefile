.DEFAULT: all
.PHONY: all dirs objs clean test

all: dirs lib/libds.so test

dirs:
	-mkdir bin/
	-mkdir lib/
	-mkdir obj/
	
lib/libds.so: objs
	gcc -shared -o lib/libds.so.1 obj/hashtable.o
	-ln -s libds.so.1 lib/libds.so

objs: obj/hashtable.o

obj/hashtable.o: src/hashtable.c include/hashtable.h
	gcc -c -o obj/hashtable.o src/hashtable.c -I include -fPIC
	
test: bin/libds_hashtable_test

bin/libds_hashtable_test: include/libds.h lib/libds.so test/hashtable/libds_hashtable_test.c
	gcc -o bin/libds_hashtable_test test/hashtable/libds_hashtable_test.c -I include -L lib -lds

clean:
	-rm bin/*
	rmdir bin/
	-rm lib/*
	rmdir lib/
	-rm obj/*
	rmdir obj/
