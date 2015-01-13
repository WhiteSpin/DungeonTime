CXX_CFLAGS = -O3 -std=c++1y -stdlib=libc++

all:
	$(CXX) $(CXX_CFLAGS) \
		-o main\
		*.cpp
