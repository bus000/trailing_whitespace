CC      =gcc
CFLAGS  =-Wall -ansi -Werror
SOURCES =$(wildcard *.c)$
OBJECTS =$(SOURCES:.c=.o)$
TARGET  =trailing_whitespace
LFLAGS  =

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LFLAGS)

%.o: %.c
	$(CC) -o $@  $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
