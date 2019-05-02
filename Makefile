TARGET = k128

CC = gcc
CFLAGS = -g -Wall
SRCDIR = src

LDFLAGS = -lm
OBJFILES = $(SRCDIR)/k128.o $(SRCDIR)/main.o $(SRCDIR)/io_handling.o 

all: $(TARGET)

tables: src/dot_tables_gen.c
	$(CC) $(CFLAGS) -o tables $(SRCDIR)/dot_tables_gen.c $(LDFLAGS)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	-rm -f $(SRCDIR)/*.o $(TARGET) *~
