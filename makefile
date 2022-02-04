output: main.o PNG_read.o filter.o BMP_write.o zpipe.o
	gcc -o my_project main.o PNG_read.o filter.o BMP_write.o libz.a zpipe.o -lz 

main.o: main.c
	gcc -c main.c

PNG_read.o: PNG_read.c
	gcc -c PNG_read.c

filter.o: filter.c
	gcc -c filter.c

BMP_write.o: BMP_write.c
	gcc -c BMP_write.c

zpipe.o : zpipe.c
	gcc -I . -c zpipe.c -lz

clean: 
	rm main.o PNG_read.o filter.o BMP_write.o zpipe.o my_project temp.bin 


