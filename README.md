# The Repool Scheduler

## Summary

Repool is a user-level command line scheduler written in C++ and built using Make. It provides a keyword argument interface to execute programs via std::system and a serialized local file. This program only supports Linux and is only tested on x86_64 architecture.

## Prerequisite Tools

- Make (for building and testing)
- libstdc++ GNU standard C++ library (musl is not tested)
- g++ (compiler can be changed in the Makefile if you use clang++ for example)
- You are running the Linux operating system
- Bash (or similar shells)
- Git

## Build Instructions

To build run these commands:
```
git clone https://github.com/Mathieu-Poirier/repool
cd repool
make
```

To run the test suite run these commands:
```
git clone https://github.com/Mathieu-Poirier/repool
cd repool
make check
```

**For subsequent calls to make you do not need to run git clone**

## Usage

Right now there is no standard API for the scheduler but the planned API will be:
```
repool job=/home/myjob exec=[Repeat12][12:00AM][Nov-4-20XX] repeat=[2 weeks] exclude=[Jul-5-20XX]
```

## Development Goals

See ```docs/Tasks.txt``` for remaining dev work

## License

MIT