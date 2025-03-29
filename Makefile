# Compiler and linker flags
CFLAGS = -std=c++17 -O2 -Wall
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -llua -lm

# Source and object directories
SRC_DIR = src
OBJ_DIR = obj
SHADER_DIR = shaders
SPV_DIR = spv

# Find all .cpp files in the src directory
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Replace .cpp with .o for object files
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Find all shader files
SHADERS := $(wildcard $(SHADER_DIR)/*.vert $(SHADER_DIR)/*.frag $(SHADER_DIR)/*.comp)

# Replace shader extensions with .spv for compiled shader files
SPV_FILES := $(SHADERS:$(SHADER_DIR)/%=$(SPV_DIR)/%.spv)

# Name of the final binary
TARGET = engine_demo

# Default target to build the program and shaders
all: $(TARGET) $(SPV_FILES)

# Rule to link the object files into the final binary
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

# Rule to compile shaders
$(SPV_DIR)/%.spv: $(SHADER_DIR)/%
	@mkdir -p $(SPV_DIR)
	glslc $< -o $@

# Clean up the build
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(SPV_DIR)

.PHONY: all clean
