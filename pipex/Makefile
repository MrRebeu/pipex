# Nom du programme
NAME = pipex

# Compilateur et options de compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Chemins
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a
INCLUDES = -I$(LIBFT_PATH) -I.

# Sources et objets
SRCS = pipex.c \

OBJS = $(SRCS:.c=.o)

# Headers
HEADER = pipex.h

# Règles
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -o $(NAME)

# Règle pour compiler la libft
$(LIBFT):
	make -C $(LIBFT_PATH)

# Règle pour compiler les objets avec accès aux headers de libft
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C $(LIBFT_PATH) clean
	rm -f $(OBJS)

fclean: clean
	make -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
