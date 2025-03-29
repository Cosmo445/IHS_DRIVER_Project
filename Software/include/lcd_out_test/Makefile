# Compilador e flags para C
CC = gcc-13
CFLAGS = -m64 -no-pie -Wall -Wextra

# Compilador para Assembly (NASM)
NASM = nasm
NASMFLAGS = -f elf64

# Nome do executável
TARGET = prog

# Arquivos objeto
OBJS = liquid_crystal.o update_data.o pulse_enable.o main.o

# Regra padrão
all: $(TARGET)

# Linkagem dos objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Regras para arquivos C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regras para arquivos Assembly
update_data.o: update_data.asm
	$(NASM) $(NASMFLAGS) $< -o $@

pulse_enable.o: pulse_enable.asm
	$(NASM) $(NASMFLAGS) $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run