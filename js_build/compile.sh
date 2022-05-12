#!/bin/sh

gcc -o example main.c -ljsbuild -L./ -lpthread -ldl
