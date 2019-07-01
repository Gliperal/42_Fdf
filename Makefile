make -C minilibx_macos/
gcc main.c -I minilibx_macos/ -L minilibx_macos -lmlx -framework OpenGL -framework AppKit
