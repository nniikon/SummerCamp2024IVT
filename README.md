SquareSolver

This simple app solves quadratic equations with rational koefficients and finds rational solutions.
The main purpose of creating it is to learn how to write good proffesional code.

There are docs, if you want to know more about program.

required libs: doxygen v1.9.1

In order to install, type this in your command line:
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install doxygen
sudo apg-get install doxygen graphwiz
```

To check that doxygen lib is successfully installed:

```
doxygen --version
```
Possible output: 1.9.1

All docs appearance config lies in Doxyfile. In order to open docs, type in terminal:
```
./bashScripts/open_docs.sh
```

Program can be run by typing this commands in terminal:
```
make run
```

If you want to run release version type this:
```
make run DEBUG=0
```

If you want to test your program on tests type this:
```
make test
```

To get additional info about terminal args you need to build program (using make) and then when executing add --help flag:
```
./building/libRun --help
```
