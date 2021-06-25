all: planetoids.cpp
	g++ -g -O2 -std=gnu++17 -static -o planetoids planetoids.cpp