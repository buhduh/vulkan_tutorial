CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Iinclude
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

SPIKE_SRC = $(wildcard spike/*.cpp)
SPIKE_BIN = $(patsubst spike/%.cpp,bin/%_spike,$(SPIKE_SRC))

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))

EXE = bin/drone

drone: $(EXE)

$(EXE): $(OBJ)
	g++ -g $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ): build/%.o: src/%.cpp
	g++ -g $(CFLAGS) -o $@ -c $^ $(LDFLAGS)

spike: $(SPIKE_BIN)

bin/%_spike: spike/%.cpp
	g++ $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf build/* bin/*

clean_spikes:
	rm -rf bin/*_spike

.PHONY: test clean spike
