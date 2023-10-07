# whexd (Windows HEX Dump)
This project is a subset of the <code>hexdump</code> utility found in Unix systems, for Windows systems.

I got the idea of this toy project after really being in need of a command-line hexdump tool in Windows for another project I was working on.

# Usage
You can include the header file <code>whexd.h</code> in your project. <br>
Compile the project and use the executable, compilation should not be a problem since it is standard C.

# Currently supported features
<ul>
   <li>Skipping N bytes from the start of the file</li>
   <li>Reading up to N bytes from starting point</li>
   <li>Verbose printing</li>
   <li>Printing 16-bit values in hexadecimal format</li>
   <li>Printing 16-bit values in octal format</li>
   <li>Printing 16-bit values in decimal format</li>
   <li>Printing 8-bit values in octal format</li>
   <li>Printing 8-bit values in character format</li>
   <li>Printing 8-bit values in canonical format (hex+ascii)</li>
</ul>

~~I will add the rest of the printing formats in the future.~~ -> **DONE** <br>
I will NOT however add the <code>--color</code>, <code>--format</code> and <code>--format-file</code> options.

~~One last feature i would like to add is the ability to redirect the program's output to whatever stream you pass to it.~~ -> **DONE** <br> <br>

Version 1.0 -> Feature Complete! <br>
Only bug fixes from now on.
