CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=vertex.c quaternion.c matrix.c ft_draw_line.c
OBJS=vertex.o quaternion.o matrix.o ft_draw_line.o
RM=rm -rf
NAME=librendering.a
AR=ar rcs

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

vertex.o: vertex.h
quaternion.o: quaternion.h
matrix.o: matrix.h vertex.h
ft_draw_line.o: mlx_util.h
