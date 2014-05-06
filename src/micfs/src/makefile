CFLAGS=-g  -Wall
main:main.o ga.o sffs.o mic.o mic_matrix.o fcbf.o para_chk.o read_content.o
	gcc -o main main.o ga.o sffs.o mic.o mic_matrix.o fcbf.o para_chk.o read_content.o $(CFLAGS)
main.o:main.c main.h
	gcc -c main.c $(CFLAGS)
ga.o:ga.c ga_test.c ga.h
	gcc -c ga.c ga_test.c $(CFLAGS)
sffs.o:sffs.c sffs_test.c sffs.h
	gcc -c sffs_test.c sffs.c $(CFLAGS)
mic.o:mic.c mic.h
	gcc -c mic.c $(CFLAGS)
mic_matrix.o:mic_matrix.c mic_matrix.h
	gcc -c mic_matrix.c $(CFLAGS)
fcbf.o:fcbf.c fcbf.h
	gcc -c fcbf.c $(CFLAGS)
para_chk.o:para_chk.c para_chk.h
	gcc -c para_chk.c $(CFLAGS)
read_content.o:read_content.c read_content.h
	gcc -c read_content.c $(CFLAGS)
clean:
	rm main *.o


