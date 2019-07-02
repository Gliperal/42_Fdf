CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=fdf.c read_file.c vertex.c quaternion.c main.c matrix.c ft_draw_line.c
OBJS=fdf.o read_file.o vertex.o quaternion.o main.o matrix.o ft_draw_line.o
LIBMLX=minilibx_macos
LIBFT=libft
RM=rm -rf
NAME=fdf

vpath libmlx.a $(LIBMLX)
vpath libft.a libft

$(NAME): $(OBJS) -lmlx -lft
	$(CC) $(OBJS) -L $(LIBMLX) -lmlx -L $(LIBFT) -lft $(FRAMEWORKS) -o $(NAME)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) $(OBJS)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean
	make -C $(LIBMLX) clean

re: fclean all

$(LIBMLX)/libmlx.a:
	make -C $(LIBMLX)

libft/libft.a:
	make -C $(LIBFT)

vertex.o: vertex.h
quaternion.o: quaternion.h
fdf.o: map.h
read_file.o: map.h vertex.h -lft
matrix.o: matrix.h vertex.h
ft_draw_line.o: mlx_util.h -lft

main.o: main.c mlx_util.h
	$(CC) $(CFLAGS) -I $(LIBMLX) -c main.c
