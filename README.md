# readme

## What is ASET?
ASET is a small project I made for handling file loading and writing, and managing loaded files (assets) in the executables' memory. It is primarily intended to be used in video games who need to load different types of hundreds of asset files such as textures, models, configurations and so on and access them whenever they are required. ASET aims to simplify file loading and helps to avoid loading same file multiple times unnecessarily.

## TODOs:
- Multithreading support (non-blocking load functions).
- Improve callbacks, provide features to measure performance (memory use, load time, load progress).
- Add a way to report loader and writer errors.
- Possibly wrap loader and writer functions in a class using OOP for error handling and making it easier to multi-thread it.