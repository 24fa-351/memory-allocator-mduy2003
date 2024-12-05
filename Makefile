system_malloc:
	gcc -Wall -g -DSYSTEM_MALLOC memtest.c -o test_program_system

custom_malloc:
	gcc -Wall -g memtest.c heap.c -o test_program_custom
	
clean:
	rm -f test_program_system test_program_custom heap.o
