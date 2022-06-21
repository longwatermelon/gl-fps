# gl-fps
First person shooter

# Controls
* WASD: move
* Space: jump
* Mouse: rotate
* Left mouse button: shoot
* Right mouse button: scope

https://user-images.githubusercontent.com/73869536/174900245-94e88940-45eb-43a0-b0a1-93a7d330b317.mp4

# Building
## Linux
```
git clone https://github.com/longwatermelon/gl-fps
cd gl-fps
make
./a.out
```

## MacOS
```
git clone https://github.com/longwatermelon/gl-fps
cd gl-fps
make CFLAGS=-Wno-error=unused-command-line-argument INC=-I$(brew --prefix)/include LIBS=-L$(brew --prefix)/lib
./a.out
```

