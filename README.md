# Arya

Arya, my own virtual assistant. And more than it.

## Install dependencies

```
sudo snap install --classic code
sudo apt install git cmake build-essential libopencv-dev libx11-dev libxrandr-dev libxcursor-dev libxi-dev libopengl-dev libgl1-mesa-dev libudev-dev libfreetype6-dev libvorbis-dev libogg-dev libflac-dev
```

## Clone

```
git clone git@github.com:DonatNathan/arya.git
cd arya
```

## Install externals

Create and move to external folder.
```
mkdir external
cd external
```

Clone and get a whisper model.
```
git clone https://github.com/ggml-org/whisper.cpp.git
cd whisper.cpp
sh ./models/download-ggml-model.sh base.en
```

## Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Run

Classic mode:
```
./bin/Arya
```

Debug mode (no speaking, command line):
```
./bin/Arya --debug
```
