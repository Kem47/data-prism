# Data-Prism (WIP)

![Data-Prism](data_prism_small.png)

Do you hate fixed-width files? Have they been a thorn in your side? When you received a file to analyse only to find out that it was a fixed-width file did you reconsider working on it?

Data-Prism is a small program that is used to cut up a fixed width data file into its component record types and columns. It takes in the data file, a spec file for the data file and the record types that the user would like to be saved. It then saves each record type into its own .csv file. It makes sure that these csv files are Excel-friendly by limiting the number of rows that can be written into a single csv file.

As part of this I have also created a user-friendly spec file format/layout. Please see spec_file_template. I've chosen not to go for conventional formats like JSON or YAML because they have a barrier to entry, especially JSON. The spec format used in this project is easy to both prepare and prepare.

The goals of this project to:
- Provide an extremely fast and a memory effecient way of transforming a fixed-width file into a user-friendly form.
- Provide a user-friendly way of working with a fixed-width file for those who do not have experience with or access to data wrangling tools like Python, MS Access or SQL.
- Practice writing a project in C. This is my CS50 Introduction to Computer Science Final Project.

### Current progress:
- [x] Get the inputs.
- [x] Parse the spec file and save into memory (spec.c).
- [x] Parse the data file using the specs and save the record type into individual files (parse.c).
- [ ] (WIP) Test accuracy of data.
- [ ] Memory cleanup.
- [ ] General code cleanup, refactoring and optimisation.


## Getting Started

Instructions for how to build and run the project.

### Prerequisites

Before you can build and run the project, you need to install the following libraries:

- zlib
- cs50

#### Installing zlib

To install zlib on Ubuntu, you can run the following command:

```bash
sudo apt-get install zlib1g-dev
```
For other operating systems, you can download the zlib source code from the [official website](https://zlib.net/) and follow the installation instructions.

#### Installing cs50

To install cs50, you can follow the instructions on the [official website](https://cs50.readthedocs.io/libraries/cs50/c/).

Make sure to install the C version of the library.

Once you have installed these libraries, you should be able to build and run the project.


### Building

To build the project, you can use the following command:
```bash
make
```

This will compile the source code and generate an executable file.

By default, the make command will include all logging outputs. If you want to build the project without logging outputs, go into the Makefile and remove ```-DLOGGING``` from ```CFLAGS```. This will compile the source code without logging outputs and generate an executable file.

Once you have built the project, you should be able to run it using the instructions provided in the "Running" section.


### Running

To run the program, you can use the following command:
```bash
./main <DATA_FILE_NAME> <SPEC_FILE_NAME> RT_NO RT_NO RT_NO
```
Replace <DATA_FILE_NAME> with the name of the data file you want to parse, and <SPEC_FILE_NAME> with the name of the spec file that describes the data file format. Replace RT_NO RT_NO RT_NO with the record types you want to extract from the data file. For example, if you want to extract record types 1, 2, and 3, you can use the following command:
```bash
./main data.txt spec.txt 1 2 3
```
On record types:
- Your data file should have record types as the first characters of each row.
- Your record types should be of the same length in all records. 
- When running make sure that the record types you input are of the same length as in your data file. So if you have leading zeroes in your record type then include the leading zeroes.
- The record types should be consistent in your spec file.

If you want to extract all record types from the data file, you can use the following command instead:
```bash
./main <DATA_FILE_NAME> <SPEC_FILE_NAME> all
```

Once you have run the program, it will generate output files for each record type that you specified. The output files will be named <DATA_FILE_NAME>_RT_<RT_NO>.txt, where <DATA_FILE_NAME> is the name of the data file and <RT_NO> is the record type number.

## Usage

- label column as 'blank' if you want the program to skip it and not write it
- all the keywords used in the spec file can be changed as long as the same changes are made in configs.h
- write 'all' instead of the record types in the input in order to get all the record types that are in the spec file.
- make sure record type lengths are the same across everything - spec file ('rt:' header and record_type column length), program inputs and the data file itself.
- make sure that your column widths add up to the row length of your data file.
- in the spec file you can group the record types that have the same specs.
- the program will check that the extension of your data_file is present in the list following 'filename:' label in the spec file. if not there will be an error
- if you do not input arguments correctly when running the program manual mode will be suggested and can be enabled by following the prompts in the CLI.


### Spec File

Details of the spec file.

## Contributing

This is a personal project. I know there are probably many things in my code that can be improved, are damn right dumb or commit programming war crimes. However this is my first C project so you'll forgive me. Please raise an issue with your suggestion but please do not create pull requests. I would like a chance to fix errors or make improvements myself.

## License

This project is licensed under the [LICENSE NAME] license - see the [LICENSE.md](LICENSE.md) file for details.