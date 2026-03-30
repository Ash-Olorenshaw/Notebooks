#!/bin/bash

FLAGS=""

if [[ $1 == "debug" ]]; then
	FLAGS="-g -fsanitize=address -fno-omit-frame-pointer "
fi

gcc $FLAGS $(pkg-config --libs --cflags libadwaita-1 libzip) main.c types.c zipread.c folder_button.c tree_view.c ./zip/entries.c ./zip/string.c -o out

./out

echo "Build complete: 'out'"
