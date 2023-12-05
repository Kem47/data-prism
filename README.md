# Data-Prism

![Data-Prism](data_prism_small.png)

Do you hate fixed-width files? Have they been a thorn in your side? When you received a file to analyse only to find out that it was a fixed-width file did you reconsider working on it?

Data-Prism is a small program that is used to cut up a multi-record fixed-width data file into its component record types and columns. It takes in the data file, a spec file for the data file and the record types that the user would like to be saved. It then saves each record type into its own .csv file. It makes sure that these csv files are Excel-friendly by limiting the number of rows that can be written into a single csv file.

As part of this I have also created a user-friendly spec file format/layout. Please see spec_file_template. I've chosen not to go for conventional formats like JSON or YAML because they have a barrier to entry, especially JSON. The spec format used in this project is easy to both prepare and prepare.

The goals of this project to:
- Provide an extremely fast and a memory effecient way of transforming a fixed-width file into a user-friendly form.
- Provide a user-friendly way of working with a fixed-width file for those who do not have experience with or access to data wrangling tools like Python, MS Access or SQL.
- Practice writing a project in C. This is my CS50 Introduction to Computer Science Final Project.


## Getting Started

Instructions for how to build and run the project.

### Prerequisites

Before you can build and run the project, you need to install the following libraries:

- zlib
- cs50

#### Installing ```zlib```

To install zlib on Ubuntu, you can run the following command:

```bash
sudo apt-get install zlib1g-dev
```
For other operating systems, you can download the zlib source code from the [official website](https://zlib.net/) and follow the installation instructions.

#### Installing ```cs50```

To install cs50, you can follow the instructions on the [official website](https://cs50.readthedocs.io/libraries/cs50/c/).

Make sure to install the C version of the library.

Once you have installed these libraries, you should be able to build and run the project.


### Building

To build the project, you can use the following command:
```bash
make
```

This will compile the source code and generate an executable file.


By default, the make command will not include all logging outputs. If you want to build the project with logging outputs, go into the Makefile and add ```-DLOGGING``` to ```CFLAGS```. This will compile the source code with logging outputs and generate an executable file.

Once you have built the project, you should be able to run it using the instructions provided in the "Usage" section.


## Usage
### Quick Video Guide:


<a href="https://youtu.be/HouhoIG3zjc" title="A Quick Guide">
    <img src="https://img.youtube.com/vi/HouhoIG3zjc/hqdefault.jpg" alt="A Quick Guide">
</a>

### Running

To run the program, you can use the following command:
```bash
./main <DATA_FILE_NAME> <SPEC_FILE_NAME> RT_NO RT_NO RT_NO
```
Replace <DATA_FILE_NAME> with the name of the data file you want to parse, and <SPEC_FILE_NAME> with the name of the spec file that describes the data file format. Replace RT_NO RT_NO RT_NO with the record types you want to extract from the data file. For example, if you want to extract record types 1, 2, and 3, you can use the following command:
```bash
./main data.txt spec.txt 01 02 03
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


### Usage Tips

- First and foremost, the program only works on a mixed record types fixed-width files.
- Run the program like this "./data-prism-linux input.file spec.file all" for all record types to be output.
- or run it like this "./data-prism-linux input.file spec.file 003 001 023 999" for a choice of record types to be output.
- or run it like this "./data-prism-linux" to enable the manual entry mode that will guide you through the inputs.
- The input data file can be uncompressed data files or data files compressed with gzip only. Other compression methods are not supported.
- The file format of the data file should be in the name of that file and should be in the spec file after "filename:".
- All record types should be of the same length across the entire data file and in all of the specs.
- In the spec file you can group the record types that have the same specs.
- Make sure that your column widths add up to the row length of your data file.
- An important requirement of the data file is that the first 1 or more characters should be the record type. Unfortunately it does not support a fixed-width file that has non-record type first column.
- There is a feature where columns named 'blank' are ignored and not output.
- If the data file contains columns that contain text and that text could contain commas it is advisabl to recomplile a tsv file by changing the OUTPUT_FILETYPE to 'tsv' and DELIMITER to '\t'.
- The output file is encoded in 'UTF-8'
- All the keywords used in the spec file can be changed as long as the same changes are made in configs.h


### configs.h

The configs.h header file contains some configs that can be changed to make adjust the program behaviour.

```C
#define RT_FLAG "rt:"               the string its looking for in the spec file to read in the record types.
#define FILENAME_FLAG "filename:"   the string its looking for in the spec file to read in the file format of the data file that this spec is for.
#define MAX_WRITE_ROWS 500000       the maximum number of rows of data that the output files can have.
#define BLANK_COLUMN_NAME "blank"   the name of the columns that can be skipped in the data file.
#define DELIMITER ','               the delimiter used to separate the columns in the output files.
#define OUTPUT_FILETYPE "csv"       the name of the output file format. this and the above rows should be changed in tandem.
#define MEMORY_LIMIT 100000000      the number of bytes that all the FILE buffers added up should be below.
#define PROGRAM_NAME "data-prism"   the name of the program.
```

### Spec File

The format of the specs should be as follows:
```
filename: ABC XYZ FOO123

rt: record_type ...
record_type_column, column_name, ...
record_type_column_width, column_width, ...
```


## TODO

Here is a list of things that I still would like to add. I don't know when I'll have a chance to work on these but I'm hoping to add these 

- Ability to input a single record fixed-width file i.e. a file that does not have a record type as the first characters of each line.
- Add handling for wide chars.
- Add directory with data files as input.
- Add the ability to input configs as input. Things like output file format.
- Add some simple GUI. Something that's enough to step away from CLI. This one is ambitious and I'll have to look into the best and simplest way of building GUI in C.



### Kemal Atayev
### December 2023