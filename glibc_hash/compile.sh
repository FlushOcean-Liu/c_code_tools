#!/bin/sh

gcc -o example ghash_main.c `pkg-config --libs --cflags glib-2.0`
