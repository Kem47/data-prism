


Spec file usage:
- there is room for comments using 
- 

rt: XX
[column names list comma separated]
[column width list comma separated]



- The program currently support reading of data files that are either uncompressed or compressed only using gzip. If your file has been comnpressed using any other compression algorithm please unzip it first and then provide it as input.
- File size supported by the pogram ...
- type should be either fwf, tsv, csv. However if it's not fwf this program is not much use to you. tsv and csv are easy enough to open in excel.
- Make sure that your record_type columns are the same length/width in each rt. #
- Make sure that you the number of column names and widths is the same.

No longer true.
- the program does not currently support unzipping of very large files. Any file that may be too large to be held in its entirety within memory will crash the program.