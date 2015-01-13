CPPOPTIONS := -O3 -std=c++1y -stdlib=libc++

all:
	clang++ ${CPPOPTIONS}\
		-o main\
		*.cpp
