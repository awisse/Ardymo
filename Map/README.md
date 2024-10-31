# Instructions

The *src* directory containts symbolic links on ext4 to files in 
`../Ardymo`. This means the application **won't compile on Windows or Mac as
is**. For the compilation to work, either replace the symbolic links with links
relevant to your platform or copy the files over from the `../Ardymo` directory.

I have found no other way to include the shared code. The ArduinoIDE and
arduino-cli seem to provide no other way to instruct the compiler to include a
source directory other than `src` directly inside the sketch.

If there is a better way, please open an issue or a pull request with the
solution.
