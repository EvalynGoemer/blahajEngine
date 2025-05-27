# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    # Linux settings
    CXX = g++
    CFLAGS = -std=c++17 -O2 -Wall
    LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -llua -lm
    INCLUDES =
    LIBPATHS =
else ifneq (,$(findstring MINGW64,$(UNAME_S)))
    # Windows MSYS2 MinGW64 settings
    MINGW64 = /mingw64
    CXX = $(MINGW64)/bin/g++
    CFLAGS = -std=c++17 -O2 -Wall -I$(MINGW64)/include
    LDFLAGS = -L$(MINGW64)/lib -lglfw3 -lvulkan-1 -llua -lm
    INCLUDES = -I$(MINGW64)/include
    LIBPATHS = -L$(MINGW64)/lib
else
    $(error Unsupported platform: $(UNAME_S))
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj
SHADER_DIR = shaders
SPV_DIR = spv

# Sources and objects
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Shaders and compiled shaders
SHADERS := $(wildcard $(SHADER_DIR)/*.vert $(SHADER_DIR)/*.frag $(SHADER_DIR)/*.comp)
SPV_FILES := $(SHADERS:$(SHADER_DIR)/%=$(SPV_DIR)/%.spv)

TARGET = engine_demo

all: $(TARGET) $(SPV_FILES)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LIBPATHS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(SPV_DIR)/%.spv: $(SHADER_DIR)/%
	@mkdir -p $(SPV_DIR)
	glslc $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(SPV_DIR)

.PHONY: all clean
