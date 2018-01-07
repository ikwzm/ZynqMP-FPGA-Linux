### Build udmabuf Device Drivers and Services Package

#### Donwload Sources from github

```
shell$ cd ./udmabuf
shell$ git clone --depth=1 -b v0.9.0 https://github.com/ikwzm/udmabuf
```

#### Build Device Driver debian package

```
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ sudo debian/rules binary
```

