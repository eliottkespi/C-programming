CC=gcc
CFLAGS=-Wall -ansi -pedantic -g -g3  


assembler: assembler.c assembler.h error.h assembler_logging.o assembler_tables.o assembler_assembler.o assembler_images.o assembler_io_utils.o assembler_files.o
	$(CC) $(CFLAGS) assembler_logging.o assembler_tables.o assembler_assembler.o assembler_images.o assembler_io_utils.o assembler_files.o assembler.c -o assembler

assembler_logging: assembler_logging.c assembler.h
	$(CC) -c $(CFLAGS) assembler_logging.c -o assembler_logging.o

assembler_tables: assembler_tables.c assembler.h
	$(CC) -c $(CFLAGS) assembler_tables.c -o assembler_tables.o

assembler_images: assembler_images.c assembler.h
	$(CC) -c $(CFLAGS) assembler_images.c -o assembler_images.o

assembler_assembler: assembler_assembler.c assembler.h
	$(CC) -c $(CFLAGS) assembler_assembler.c -o assembler_assembler.o

assembler_io_utils: assembler_io_utils.c assembler.h
	$(CC) -c $(CFLAGS) assembler_io_utils.c -o assembler_io_utils.o

assembler_files: assembler_files.c assembler.h
	$(CC) -c $(CFLAGS) assembler_files.c -o assembler_files.o


clean:
	rm -f ./*.o *~ a.out assembler

