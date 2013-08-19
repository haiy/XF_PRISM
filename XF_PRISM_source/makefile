CFLAGS=-g -lm -Wall
main.exe:main.o ga.o sffs.o mic.o mic_matrix.o fcbf.o para_chk.o read_content.o
	cc -o main main.o ga.o sffs.o mic.o mic_matrix.o fcbf.o para_chk.o read_content.o $(CFLAGS)
main.o:main.c main.h
	cc -c main.c $(CFLAGS)
ga.o:ga.c ga_test.c ga.h
	cc -c ga.c ga_test.c $(CFLAGS)
sffs.o:sffs.c sffs_test.c sffs.h
	cc -c sffs_test.c sffs.c $(CFLAGS)
mic.o:mic.c mic.h
	cc -c mic.c $(CFLAGS)
mic_matrix.o:mic_matrix.c mic_matrix.h
	cc -c mic_matrix.c $(CFLAGS)
fcbf.o:fcbf.c fcbf.h
	cc -c fcbf.c $(CFLAGS)
para_chk.o:para_chk.c para_chk.h
	cc -c para_chk.c $(CFLAGS)
read_content.o:read_content.c read_content.h
	cc -c read_content.c $(CFLAGS)
clean:
	del main *.o


