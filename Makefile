TARGET = k128

CC = gcc
CFLAGS = -g -Wall
SRCDIR = src

LDFLAGS =
OBJFILES = $(SRCDIR)/k128.o $(SRCDIR)/main.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	-rm -f $(SRCDIR)/*.o $(TARGET) *~
