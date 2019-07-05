CC=gcc
CFLAGS=-Wall -Wextra -Werror
FRAMEWORKS=-framework OpenGL -framework AppKit
SRC=vertex.c quaternion.c matrix1.c matrix2.c ft_draw_line.c camera1.c camera2.c screen.c
OBJS=vertex.o quaternion.o matrix1.o matrix2.o ft_draw_line.o camera1.o camera2.o screen.o
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
matrix.o: matrix.h vertex.h quaternion.h
ft_draw_line.o: mlx_util.h cpoint.h
camera1.o: camera.h matrix.h
camera2.o: camera.h matrix.h
