#!/bin/bash

FLAGS=""
FILES="markdown/parse.c markdown/setup.c markdown/utils.c widgets/folder_button.c zip/entries.c zip/string.c arrays.c globals.c string_utils.c tree_view.c types.c zipread.c"

if [[ $1 == "debug" ]]; then
	FLAGS="-g -fsanitize=address -fno-omit-frame-pointer "
fi

gcc $FLAGS $(pkg-config --libs --cflags libadwaita-1 libzip) main.c $FILES -o out

./out

echo "Build complete: 'out'"
