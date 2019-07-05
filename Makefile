CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=fdf.c render.c transform_map_to_screen.c map_read.c map_color.c input.c input_handle_loop.c input_handlers.c misc.c
OBJS=fdf.o render.o transform_map_to_screen.o map_read.o map_color.o input.o input_handle_loop.o input_handlers.o misc.o
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

fdf.o: input.h param.h misc.h -lmlx
map_read.o: map.h
map_color.o: map.h
input.o: input.h input_handlers.h -lmlx
input_handle_loop.o: input.h
render.o: param.h map.h -lft -lmlx
transform_map_to_screen.o: map.h param.h -lrendering
misc.o: input.h
