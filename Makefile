lib.so: src.c
	clang -Wall -Wextra -Wpedantic -fPIC -shared -o lib.so src.c
