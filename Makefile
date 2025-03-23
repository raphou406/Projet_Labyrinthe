# Nom de l'exécutable
TARGET = main

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -Werror -std=c99

# Répertoires
SRCDIR = src
OBJDIR = bin

# Fichiers source
SRCS = $(wildcard $(SRCDIR)/*.c)

# Fichiers objets (dans bin/)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Cible par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour compiler les fichiers .c en fichiers .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Créer le répertoire bin s'il n'existe pas
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Nettoyage des fichiers objets
clean:
	\rm -rf $(OBJDIR)

# Nettoyage complet (y compris l'exécutable)
distclean: clean
	rm -f $(TARGET)

.PHONY: all clean distclean
