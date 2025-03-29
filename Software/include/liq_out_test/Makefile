# Compilador e flags para C
CC = gcc-13
CFLAGS = -m32 -no-pie -Wall -Wextra

# Compilador para Assembly (NASM)
NASM = nasm
NASMFLAGS = -f elf32

# Nome do executável
TARGET = prog

# Arquivos objeto
OBJS = main.o liquid_crystal.o update_data.o

# Regra padrão
all: $(TARGET)

# Regra para linkar tudo
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para arquivos C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra ESPECÍFICA para update_data.asm (Assembly)
update_data.o: update_data.asm
	$(NASM) $(NASMFLAGS) $< -o $@

# Limpar
clean:
	rm -f $(OBJS) $(TARGET)

# Executar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run