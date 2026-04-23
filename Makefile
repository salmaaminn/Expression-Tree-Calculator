all: obj/heap.o obj/preorder.o obj/postorder.o obj/inorder.o obj/calculate.o bin/heap bin/preorder bin/postorder bin/inorder bin/calculate

obj/heap.o: src/heap.c
	clang -std=c99 -Wall -pedantic -I./src -c src/heap.c -o obj/heap.o

obj/preorder.o: src/preorder.c
	clang -std=c99 -Wall -pedantic -I./src -c src/preorder.c -o obj/preorder.o

obj/postorder.o: src/postorder.c
	clang -std=c99 -Wall -pedantic -I./src -c src/postorder.c -o obj/postorder.o

obj/inorder.o: src/inorder.c
	clang -std=c99 -Wall -pedantic -I./src -c src/inorder.c -o obj/inorder.o

obj/calculate.o: src/calculate.c
	clang -std=c99 -Wall -pedantic -I./src -c src/calculate.c -o obj/calculate.o

bin/heap: obj/heap.o
	clang obj/heap.o -lm -o bin/heap

bin/preorder: obj/preorder.o
	clang obj/preorder.o -lm -o bin/preorder

bin/postorder: obj/postorder.o
	clang obj/postorder.o -lm -o bin/postorder

bin/inorder: obj/inorder.o
	clang obj/inorder.o -lm -o bin/inorder

bin/calculate: obj/calculate.o
	clang obj/calculate.o -lm -o bin/calculate

clean:
	rm -f obj/*.o

cleaner:
	rm -f obj/*.o bin/heap bin/preorder bin/postorder bin/inorder bin/calculate