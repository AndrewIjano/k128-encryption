TARGET = k128

CC = gcc
CFLAGS = -g -Wall
SRCDIR = src

LDFLAGS = -lm
OBJFILES = $(SRCDIR)/k128.o $(SRCDIR)/main.o $(SRCDIR)/io_handling.o $(SRCDIR)/utils.o $(SRCDIR)/entropy_meter.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	-rm -f $(SRCDIR)/*.o $(TARGET) *~
