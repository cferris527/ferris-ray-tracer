# Compiler & flags & GIMP path to open render
CXX = g++
CXXFLAGS = -Wall -fopenmp
GIMP = "C:\Program Files\GIMP 2\bin\gimp-2.10.exe"

# Target executable & output file
TARGET = frt
OUTPUT = image.ppm

# Target that will be built when you run `make`
all: $(OUTPUT)

# Rule to build the executable
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

# Rule to generate the output file & open with GIMP
$(OUTPUT): $(TARGET)
	$(TARGET) > $(OUTPUT)
	$(GIMP) $(OUTPUT)

# Clean up build files
clean:
	rm -f $(TARGET) $(OUTPUT)

