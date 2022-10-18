#!/bin/bash

gcc c_get_python.c   -I/usr/include/python3.8/ -L /usr/lib/x86_64-linux-gnu/ -lpython3.8 -g
