### Build fclkcfg Device Drivers and Services Package

#### Donwload Sources from github

```
shell$ cd ./fclkcfg
shell$ git clone --depth=1 -b v0.0.3 https://github.com/ikwzm/fclkcfg
```

#### Build Device Driver debian package

```
shell$ export ARCH=arm64
shell$ export CROSS_COMPILE=aarch64-linux-gnu-
shell$ sudo debian/rules binary
```

