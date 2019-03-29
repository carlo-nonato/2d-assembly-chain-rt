# 2D Assembly Chain Real-Time Simulation

*Disclaimer: This is a project with educational purposes only as it's the result of a real-time computing class.*

It's a simple 2D simulation of an assembly chain with a conveyor belt and two robots driven by real time tasks.
## Original requirements
Assembly chain. Simulate an assembly chain where objects move on a conveyor belt and are mounted by a set of robot stations. Required stages are: visual recognition, selection (an object with anomaly is discarded), assembling (a piece from a stack is mounted on the object).
## Dependencies
- Qt5 (https://www.qt.io/)
- OpenCV4 (https://www.opencv.org/)
## Compiling and executing
Use `qmake` to generate the Makefile then simply run `make`.
```console
$ qmake AssemblyChain.pro
[...]
$ make
```
The project executable will be placed under `bin/`.
## Authors
- Carlo Nonato
- Lorenzo Fiorani
