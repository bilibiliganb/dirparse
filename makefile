CXX = g++

CXXFLAGS = -lsqlite3

TARGET1 = searchall
SOURCES1 = search.cpp

TARGET2 = dirparse
SOURCES2 = dirparse.cpp

TARGET3 = bar
SOURCES3 = bar.cpp

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SOURCES1)
	$(CXX) -o $(TARGET1) $(SOURCES1) $(CXXFLAGS) 

$(TARGET2): $(SOURCES2)
	$(CXX) -o $(TARGET2) $(SOURCES2) $(CXXFLAGS) 

$(TARGET3): $(SOURCES3)
	$(CXX) -o $(TARGET3) $(SOURCES3) $(CXXFLAGS) 

clean:
	rm -f $(TARGET1)
	rm -f $(TARGET2)
	rm -f $(TARGET3)
dbg: 
	$(CXX) -g -o $(TARGET1) $(SOURCES1) $(CXXFLAGS) 
	$(CXX) -g -o $(TARGET2) $(SOURCES2) $(CXXFLAGS) 
	$(CXX) -g -o $(TARGET3) $(SOURCES3) $(CXXFLAGS) 