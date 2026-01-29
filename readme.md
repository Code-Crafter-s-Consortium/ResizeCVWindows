
# Resize Windows in CABINET VISION.

This is an application to resize some of the terribly cramped windows in CABINET VISION.  
Currently supported is the drawing sheets window, and the tool selection window in the CAM editor.

## Compiling

You will need a C compiler such as gcc. Get the gcc tool chain here: [MinGW](https://sourceforge.net/projects/mingw/)

If your path to `MINGW\bin` is the same as mine, you should just be able to run the `.\build` command in the project directory and the exe file will be output into the `output` directory.  
With the C++ workload, Visual Studio should also be able to automatically compile and link the project.  
For the complete process, see below.

<details>
  <summary>
    Compilation Details
  </summary>


The complete compilation is a two step process. A C compiler (like MinGW gcc) must be installed on your system.
### Step 1
Resource.rc must be compiled into an object `.o` file the the MinGW gcc compiler can link in.
Following is the command for compiling a `.rc` file into a `.o` file. Note you must use the absolute path to windres, or have it added into your environment variables.
```
windres resource.rc -o resource.o -O coff
```
### Step 2
Now all the project files must be compiled together.
Here is the command we'll use for the final build. Note you must use the absolute path to gcc, or have it added into your environment variables.
```
gcc main.c resource.o -o "Resize CV Windows.exe" -mwindows
```

</details>


## Releases
### V1.0.0
Supports the resizing of the drawing sheets window.
