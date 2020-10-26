TARGET  = memgrind
CC      = gcc
CFLAGS  = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) *.o *.a *.dylib *.dSYM
