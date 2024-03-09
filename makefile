CXX = g++

CXXFLAGS = -lsqlite3

TARGET1 = searchall
SOURCES1 = search.cpp

TARGET2 = dirparse
SOURCES2 = dirparse.cpp

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(SOURCES1)
	$(CXX) -o $(TARGET1) $(SOURCES1) $(CXXFLAGS) 

$(TARGET2): $(SOURCES2)
	$(CXX) -o $(TARGET2) $(SOURCES2) $(CXXFLAGS) 

clean:
	rm -f $(TARGET1)
	rm -f $(TARGET2)
