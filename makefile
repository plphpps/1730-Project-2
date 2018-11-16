all: cat head wc

cat: cat.cpp
	g++ -o cat cat.cpp

head: head.cpp
	g++ -o head head.cpp

wc: wc.cpp
	g++ -o wc wc.cpp
