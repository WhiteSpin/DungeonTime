CXX_CFLAGS = -O3 -std=c++1y -stdlib=libc++
LINK = -l cxxrt

all:
	$(CXX) $(CXX_CFLAGS) $(LINK)\
		-o main\
		*.cpp
