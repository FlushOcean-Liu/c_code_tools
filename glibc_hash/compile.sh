#!/bin/sh

gcc -o example ghash_main.c `pkg-config --libs --cflags glib-2.0`

gcc -o key_int ghash_key_int.c `pkg-config --libs --cflags glib-2.0`
