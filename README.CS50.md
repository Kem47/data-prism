Your README.md file should 
- be minimally multiple paragraphs in length
- should explain 
  - what your project is, 
  - what each of the files you wrote for the project contains and does
- if you debated certain design choices, explaining why you made them. 
- document your project thoroughly.



# DATA-PRISM
#### Video Demo:  <URL HERE>
#### Description:

The project I have chosen to do for my CS50 Intro to Computer Science Final Project is a fixed-width file parser written in C.

There are several reasons for choosing this particular project.

The first reason, I have a career in data analysis and science and I work with data on a daily basis. That means that I come across data in many different forms and shapes. However none are as difficult to deal with as fixed-width files. Typically you require some kind of advanced programming tool in order to parse and open the file for reading. In my case it's either uploading the file onto a SQL server or using Python and one of its data libraries like pandas. I found that there are many drawbacks that are attached with those methods. Firstly, both require expertise and knowledge of the tools. Many people in data analysis and insight are able to read and analyse the data but that does not mean they have the data engineering know-how to process the files for reading. Secondly, both SQL and Python require that software is installed on the computer in order to process the files. This again is a blocker for someone who does not have expertise in the subject. Usually with corporate IT it takes some time to get the required software installed and setup. The other problem that I found with SQL and Python is that they are slow, or shall we say slower than what a computer can do. There is just too much overhead for processing a simple fixed-width file. In my career I have needed to open a FWF that wasn't on any of our databases enough that it has stuck in my mind as one of the early stepping stones that I have to get through in order to begin working with that data. Sure someone could say that if I am a data professional then I should be in Python at all times. Usually I would agree, however I've dealth with enough files that were a few hundred or maybe even fewer lines. In addition sometimes you just need to look at the data and not do any advanced analysis that you need Python for. For both of those cases something simple like Excel is more than enough and it is usually available on most modern machines. So I decided that having a simple program that will take in a fixed-width data file and the column specs for it could be very beneficial not only to me, but also my colleagues and maybe other people in the wider data analysis field. 

The second reason is that I undertook CS50 in order to learn C and while doing the course I got confirmation for myself that I did not make a mistake. I enjoyed working with C and I wanted to take it out of preset problem sets given to us by the faculty staff and take it into the real world and try to tackle a real problem (close to me) from 0 to completion. I realised that if I did not do it now then I would probably never be able to motivate myself to undertake something like this again.

Straight from the start I knew exactly the components I wanted to have in this program and in most part it is what I manged to get to in the end. I've had some experience with development in the past, having used Python for many work projects and one of the main commandments I like to stick to is to start simple and build up later on. It's too easy to put this ideal version of the project on paper and set it as a goal to then realise that it is just too much to do, especially for one person. I found it is much better to get the simplest project completed that wil get you to your goal the fastest and the easiest way and only then to build on top of that all the features you wanted in the "ideal" version. In many cases by the time the project is completed those "ideal" features don't even fit or make sense while in the process of building this simplest version you get ideas for other things that fit much better.

Here is the simplest outline of my project. I wanted a single fixed-width file opened. If it is a gzip then unzip it and then open it. Then open a spec file and read in the record type and column widths. Proceed to cut it and save it into separate files for each record type.

The big feature that I had to drop pretty much initialy was this one. I wanted the program to take in a directory as an input instead of the data file and it would process all the files in that directory. I realised that it this moment before I even wrote a single line I do not know how to even do this for a single file let alone a whole directory of files. So I thought, let's focus on a single file version and then as I go I will be able to tell if I can do folder input.

Now let's get into the making of it. In my mind the project had 3 distinct parts - getting inputs from user, parsing the spec file, cutting and saving the data file.


main.c :
The initial stage is where I spent the most amount of time. This is not due to any complexity but more because of being not sure how to layout the project. What kind of functions do I need, how would the inputs from the user flow into the next parts of the program. Programming in C is not like Python where a lot can be done in one function. Every little step requires quite a lot code and thus, in most cases, should be its own function. At this point I also had an idea to include a manual entry mode for my program. Initially I did not think much about how the user would input information into the program. I just went with the thought that I would use argv and parse it for the bits I needed. However I then realised that having guided step by step inputs for those who are not familiar with CLI would be good and most importantly not difficult to implement. I used CS50 library in my code for user inputs. I did think of writing my own function to take in string user input but this was part of not over complicating the project. The get_string function was easily accessible through CS50 library so I just used it and added the writing of my own get_string function onto the list of future improvements I could do.

After overthinking the design of the program I again just went for whatever I thought of at the time - it's always possible of thinking of something better if I spent a long time on it. Unfortunately this overthinking took a lot of time. However once I did a simple layout of the program I could move on to the actual meat of it and the scary parts.

My next move was to take in the pointer to the data file and then unzip it. I found a library called zlib which is able to deal with gzip files. The reason I needed gzip is that data files are often compressed with gzip as its a common feature of linux. However this where I ran into my first big problem. There is a function called uncompress in zlib that is meant to do just that, however I was unable to get it to work. I've spent many days going through examples of code on the internet for my particular use case. Sad to say there wasn't much helpful code out there that I could use to understand where it was going wrong. It got to a point where I was afraid for this part of the project. I had 2 options in front of me. Either remove the unzipping part of the program and make the user unzip the data file before inputting it into the program or to write the manual uncompress code that zlib supports. It's essentially reading the stream of the compressed file and uncompress it as you read it. The difficulty of writing that was quite high for someone who's not very experienced with C. I chose that I did want to have unzipping so I began writing that code. As I was going through the zlib documentation I found an epiphany. It turns out zlib also has functions to open a gzip file like you would open a regular FILE in C. It's called gzopen and you can go through that pointer with functions like gzgets and gzseek which are all part of zlib library. This completely eliminated the need for uncompression. I had an entire file called unzip.c with various functions for that and I could literally remove it from my project. I guess I learnt the lesson of reading the documentation because this uncompression task took up a lot of time that I could have avoided.

There are only 2 functions here - main and manual_inputs. Everything else is in other files.


spec.c :
With uncompression not being a problem anymore I moved on to reading the spec file. I considered quite a few options here. I could either pick from a list of various files that are used to hold this type of information like JSON, YAML or INI. There are decent libraries out there for C that help with parsing of those. However, I chose to write my own. There are 2 reasons for this. I wanted something that is easily readable and writable by regular users. Something like JSON is way too complex to quickly fill in. Secondly, writing my own would allow me to control the code and format and make sure they are a perfect fit. Using an external library comes with learning that library and the file format and adhering to all those rules not just in the programming but also in the creation of these spec files in the future. I believed that writing my own and making my own code for reading it would be a not too ambitious udnertaking. I would have full control and so if I needed to make changes to fit the code or the users I could make it without too many issues.

I turned out to be right. It was challenging but I was able to create a simple file layout that had key data identifiers like "filename:" for the file types the spec was for or "rt:" for the record type. Below is a simple template:

```
filename: ABC XYZ FOO123
type: fwf


rt: [record_type] ([record_type], ...)
record_type_column, column_name, ...
record_type_column_width, column_width, ...
```

It was a matter of parsing it and saving it into memory. I initially thought of using arrays and saving the specs into some kind of array form but that proving to be difficult as their size would be variable. I could not make it dynamic and started thinking of realloc but then as soon as I thought of realloc I remembered David's introduction to linked lists and I knew that linked lists were the solution here. Once I started using linked lists it was just a matter of carefully parsing the spec file and saving things into linked lists. I have a few structs that are all connected for various things like Record Types, Column Info, Output Files and one to hold the Specs. I even managed to do a little trick to save memory. If several record types have the same column specs then they all point to the same Column linked list. It's very neat and useful but I did not know the pain they would cause later (a little foreshadowing here).

All of this is done in one big function. I also have a small function to trim whitespaces and also a function to traverse the linked lists and print out their contents. I use it for debugging and early on for checking that the specs were parsed and saved correctly.


parse.c :
Reading the data file line by line and cutting each line into columns was realtively simple. At this point I knew I was past the hardest stuff and the end was in sight. I had to be careful with line length and nul characters and where they appear. I saved the outputs into csv files as they are quite easy to write in C. They do not require any header information. Here I implemented a feature that I always wanted and thought would be quite simple. Excel annoyingly does not read more than ~1 million rows of a file. I've worked with data files that had several million rows in a specific record type. I wanted to make a feature that would write the same record type into multiple files if it was more than million lines. I was able to implement it in the end however not in the "ideal" way that I had in my head and this is the perfect example of what I meant when I talked about those "ideal" features that you have before you begin the work. 

My initial idea was to split the record type into even chunks that are under 1 million rows. So for example if the record type had 2.1m rows it would split it inot 3 files of 700k lines as that's the lowest number of files that could fit chunks of under 1m. That sounds very neat however once I was actually programming I realised (and this is something I probably could have realised before starting) that I am reading and writing the lines from the data file one at a time. That means that I am unable to know ahead of time how many rows a particular record type has and how to split it. I still implemented the general idea but now I just set an arbitrary number that when the file being written into reaches the program closes it and opens a new file (with an incremented part number) and starts writing into that one. 

There are 3 functions in parse.c. gz_parser is the main function that does the reading of the data in the data file. The other 2 are open_new_output_file that does exactly that. It does also close a previous part if the limit was reached. It does make use of the struct  that are in spec.h called OutputFileInfo. 



utils.c
One things that I kept track of as I was building the parsing of the data file and writing of the output files was speed and efficiency. I realised that when working with large amounts of data in C one had to be very careful. When writing into the FILE pointer the data was not actually written onto the disk immediately. That FILE pointer was just a space in memory where the data was kept and when it was closed the data was flushed onto the disk and actually saved. I subsequently discovered a function called fflush that forced the the data from the FILE pointer to be written on the disk. 

In making of the project there were a few helper functions that I needed and I placed them in utils.c. These were things like get_line_length which provides a simple length of a file.

However the main use of utils.c was to hold all of the functions for freeing memory. Having created elaborate linked list structs in spec.c I now needed a way to free them all to avoid memory leaks. I also needed to close all the output files that I opened. This was an unexpected difficult part of the project. From the the problem sets that were covered in CS50 I was under the impression that freeing memory was relatively simple. I coulnd't be more wrong. I wrote a simple for loop to go through the linked lists to free memory, installed Valgrind and ran it with high confidence that I did it all correctly. I had near near 1000 lines of Valgrind output with various errors. I realised that in this clean up is where the real work of C began. It was about precision in writing code. It's easy to write code that does what you want but it is much harder to write code that does it in the least chaotic way. The main problem that Valgrind identified that was the reason behind so many lines was that I blindly freed pointers in a loo irrespective of whether there was anything to free. I had to change my freeing function to several recursive functions that checked whether pointers had memory that could be freed. I also had to go and touch everything I've coded so far to make these checks easier so structs gained additional counters. 

The biggest issue was the shared pointer to ColumnInfo struct where several RecordTypeInfo structs had pointers to the same ColmunInfo struct. This meant that when I freed it in one RecordTypeInfo it was not longer available in other RecordTypeInfo structs. I had to add a counter to the first ColumnInfo in the linked list that kept track of how many RecordTypeInfo pointed to it and decremented that value as I freed RecordTypeInfos and only free that ColumnInfo linked list when it's the final one. This issue took some thinking but it was a valuable lesson.

Adjacent to the above I had a few errors about unitialised values and so I had to go back through my code and make sure I am not leaving any unitialised variables in my structs.

My overall takeaway from the experience of freeing memory and using Valgrind was that I should have definitely been using Valgrind as I develop the program. This was it would not have been so overwhelming at the end and more importantly I would have been able to make correct design decisions based on the its output as I'm actually building the program. As it turned out I had to scramble and change code that I thought was set in stone, relatively speaking, after it was all put together.





The final set of features that this program has:
- The inputs can be arguments when running the program or manual mode can be enabled that guides the user through the inputs.
- Open an uncompressed data file or a data file compressed with gzip.
- columns named 'blank' are ignored
- 




PRobably should have been doing valgrind as I build and not at the very end. Quite a lot of errors.
I keep mixing up malloc(sizeof(char *string)) and malloc(strlen(char *string)). they are not the same.