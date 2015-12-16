# QLazerDriveClient

## Integration example

To see the library in action, you can take a look at a project that I made: [lazerdrive-leaderboard-sniffer](https://github.com/vdechenaux/lazerdrive-leaderboard-sniffer)

## How to build it

You must have a Qt5 sdk installed (packages `qt5-qmake qt5-default`) with websockets support (`libqt5websockets5-dev`). To build it, you need the `build-essential` package.
```
qmake
make
make install
```
You can also use Qt Creator if you are not familiar with a shell.
