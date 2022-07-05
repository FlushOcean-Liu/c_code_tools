#!/bin/bash

gcc -o producer my_producer.c -lrdkafka -lpthread  -ldl -lssl  -lcrypto  -lm -lz -I./ -L./
