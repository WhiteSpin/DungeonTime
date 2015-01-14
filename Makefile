ifeq ($(CC),clang)
CSTD = -std=c++1y -stdlib=libc++
else
CSTD = -std=c++1y
endif
CXX_CFLAGS = -O3 $(CSTD)
#LINK = -l cxxrt

all:
	$(CXX) $(CXX_CFLAGS)\
		-o main\
		*.cpp
