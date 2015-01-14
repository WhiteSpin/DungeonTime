ifeq ($(UNAME), Linux)
CSTD = -std=c++1y -stdlib=libc++ -l cxxrt
else
CSTD = -std=c++1y -stdlib=libc++
endif

CXX_CFLAGS = -O3 $(CSTD)

all:
	clang++ $(CXX_CFLAGS)\
		-o main\
		*.cpp
