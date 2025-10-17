
A utility to resize windows in CABINET VISION.

## Installation

If your path to MINGW\bin is the same as mine, you should just be able to run the .\build command and the exe file will be output into the `output` directory.
For the complete process, see below.

The complete compilation is a two step process. A C compiler (like MinGW gcc) must be installed on your system.

### Step 1
Resource.rc must be compiled into an object (.o) file the the MinGW gcc compiler can link it in.
I have a compiled version here so you don't need to do this step, but if you ever make any changes to resource.rc, it must be recompiled into resource.o.
Following is the syntax for compiling a .rc file into a .o file. Note you must at the windres directory, or have it added into your environment variables.

```
windres resource.rc -o resource.o -O coff
```

### Step 2
Now all the project files must be compiled together.
Here is the syntax we'll use for the final build. Note you must at the gcc directory, or have it added into your environment variables.

```
gcc main.c resource.o -o "Resize CV Windows.exe" -mwindows
```


### V1.0.0
Supports the resizing of the drawing sheets window.