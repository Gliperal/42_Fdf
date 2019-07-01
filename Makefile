CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=main.c quaternion.c
OBJS=main.o quaternion.o
LIBMLX=minilibx_macos
RM=rm -rf
NAME=fdf

vpath libmlx.a $(LIBMLX)

$(NAME): $(OBJS) -lmlx
	$(CC) $(OBJS) -L $(LIBMLX) -lmlx $(FRAMEWORKS) -o $(NAME)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBMLX) clean

re: fclean all

$(LIBMLX)/libmlx.a:
	make -C $(LIBMLX)

quaternion.o: quaternion.h

main.o: main.c mlx_util.h
	$(CC) $(CFLAGS) -I $(LIBMLX) -c main.c
