#!/bin/bash

mkdir -p bin

case "$1" in
	c)
		gcc -lpthread $2 -O3 src/main.c -o bin/c
		;;
	
	atomic)
		g++ -lpthread $2 -std=c++17 -O3 src/main.cpp -o bin/atomic -DATOMIC
		;;

	cv)
		g++ -lpthread $2 -std=c++17 -O3 src/main.cpp -o bin/cv
		;;
esac

