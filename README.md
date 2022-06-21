# gl-fps
First person shooter

# Controls
* WASD: move
* Space: jump
* Mouse: rotate
* Left mouse button: shoot
* Right mouse button: scope

https://user-images.githubusercontent.com/73869536/174902304-8770bd75-b508-4811-8801-2516016d791e.mp4

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

