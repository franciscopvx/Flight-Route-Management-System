# Definição do compilador
CC=gcc

# Flags de compilação para garantir aderência ao padrão ANSI C e ativar todos os avisos
CFLAGS=-Wall -Werror -pedantic -ansi

# Nome do arquivo executável final
EXECUTABLE=rotas2024

# Dependências (arquivos header)
DEPS = aeroportoserotas.h

# Arquivos objeto resultantes da compilação dos arquivos fonte
OBJ = main.o aeroportos.o ordenacaoescalas.o rotas.o rotascomescala.o rotasdiretas.o rotasdiretascrescente.o rotasdiretasdecrescente.o

# Regra padrão para compilar o projeto completo
all: $(EXECUTABLE)

# Como transformar um arquivo .c em um arquivo .o; depende do arquivo header
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Como criar o executável a partir dos arquivos objeto
$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Como limpar os arquivos compilados e o executável
clean:
	rm -f $(OBJ) $(EXECUTABLE)
