# gl-fps
First person shooter

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

