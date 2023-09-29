# whexd (Windows HEX Dump)
This project is a subset of the <code>hexdump</code> utility found in Unix systems, for Windows systems.

I got the idea of this toy project after really being in need of a command-line hexdump tool in Windows for another project I was working on.

# Usage
You can compile the project and use the executable as you like. You can compile it using VS Code and the C/C++ Extension. The compile task is provided in <code>.vscode/tasks.json</code>. <br> <br>
You can include the header file <code>whexd.h</code> in your project.

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
</ul>

I will add the rest of the printing formats in the future. <br>
I will NOT however add the <code>--color</code>, <code>--format</code> and <code>--format-file</code> options.
