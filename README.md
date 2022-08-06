# readme

## What is ASET?
ASET is a small project of mine for handling file loading and writing, and managing loaded files (assets) in the executables' memory. It is primarily intended to be used in video games who need to load hundreds of asset files such as textures, models, configurations and so on and access them whenever they are required. ASET aims to simplify file loading and helps to avoid loading same file multiple times unnecessarily.

## TODOs:
- Multithreading support (non-blocking load functions)
- Loading assets without defining type by using metas
- Improve callbacks, provide features to measure performance