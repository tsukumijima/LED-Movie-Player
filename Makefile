CXX=g++

OBJS=led-movie-player.o
TARGET=led-movie-player

CXXFLAGS=-std=c++11
RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lSDL -lSDL_mixer

all: $(TARGET)

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

$(TARGET): $(OBJS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

install:
	cp led-movie-convert /usr/local/bin/led-movie-convert
	cp $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	$(MAKE) clean
