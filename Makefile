# ===== MCU / Compiler settings =====
MCU = attiny1616
F_CPU = 20000000UL
CC = avr-gcc
SIZE = avr-size
OBJCOPY = avr-objcopy

# ===== Flags =====
CFLAGS = -std=c11 -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra
INCLUDES = -ICode/src -ICode/lib

# ===== Sources =====
BUILD_DIR = build
SRC := $(wildcard Code/src/*.c) $(wildcard Code/lib/*/*.c)
OBJ := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))
TARGET = $(BUILD_DIR)/firmware

# ===== Rules =====
all: $(TARGET).hex

# Create build dir automatically
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/Code/src $(BUILD_DIR)/Code/lib

# Compile source files into build dir
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Link
$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
	@$(SIZE) -C $@

# Create HEX
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# ===== Flash (UPDI) =====
flash: $(TARGET).hex
	avrdude -p $(MCU) -c jtag2updi -P /dev/ttyUSB0 -U flash:w:$<

# ===== Clean =====
clean:
	@rm -rf $(BUILD_DIR)
	@echo "cleaned up build files"

compiledb:
	bear -- make all
