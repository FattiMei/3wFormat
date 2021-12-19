## Rationale
XYZ printers work with a proprietary file format known as 3w. Thanks to the work of many people on the internet, we have a better understanding of the format.
This repository collects infomation about 3w files and provides tools for conversion between gcode and 3w.

## Conversion
Before using the tools we need to compile them. All sources are in the ./src directory.

### Dependences

  * C compiler
  * Zlib
  * Nettle

### Build
Simply `make` in this directory.

### To gcode
  `./to_gcode.out filename`

Accepts a 3w file and extracts the gcode. If the file does not follow the specification, the prints an error message.
