CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=fdf.c main.c read_file.c input.c
OBJS=fdf.o main.o read_file.o input.o
LIBMLX=minilibx_macos
LIBFT=libft
LIBRENDERING=rendering
RM=rm -rf
NAME=fdf

vpath libft.a $(LIBFT)
vpath libmlx.a $(LIBMLX)
vpath librendering.a $(LIBRENDERING)

$(NAME): $(OBJS) -lmlx -lft -lrendering
	$(CC) $(OBJS) -L $(LIBMLX) -lmlx -L $(LIBFT) -lft -L $(LIBRENDERING) -lrendering $(FRAMEWORKS) -o $(NAME)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT) clean
	make -C $(LIBRENDERING) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT) fclean
	make -C $(LIBMLX) clean
	make -C $(LIBRENDERING) fclean

re: fclean all

$(LIBMLX)/libmlx.a:
	make -C $(LIBMLX)

libft/libft.a:
	make -C $(LIBFT)

rendering/librendering.a:
	make -C $(LIBRENDERING)

libs:
	make -C $(LIBMLX)
	make -C $(LIBFT)
	make -C $(LIBRENDERING)

fdf.o: map.h
read_file.o: map.h -lrendering -lft
input.o: input.h -lrendering -lft
main.o: main.c -lrendering
	$(CC) $(CFLAGS) -I $(LIBMLX) -c main.c
