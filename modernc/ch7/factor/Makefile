CC = gcc-14
CFLAGS = -Wall -Wextra -std=c23 -g

# -lm as a linker flag links in the standard math library.
# No sure if I actually need it yet.
LDFLAGS = -lm

TARGET = factor

OBJECTS = factor.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm $(TARGET) $(OBJECTS)
