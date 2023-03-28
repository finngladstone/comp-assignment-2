TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c
OBJ        = $(SRC:.c=.o) logic_ops.c bit_ops.c header.h

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET)

tests:
	gcc --coverage vm_riskxvii.c bit_ops.c logic_ops.c -o vm_riskxvii_test

run_test:
	./vm_riskxvii_coverage tests/arithmetic/arithmetic.mi
	./vm_riskxvii_coverage tests/console_writes/console_writes.mi
	./vm_riskxvii_coverage tests/dump/dump.mi
	./vm_riskxvii_coverage tests/test_invalid/test_invalid.mi 

	gcovr
	
# note return value of 1 is expected behaviour!
clean:
	rm -f *.o *.obj $(TARGET)
