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

The initial stage is where I spent the most amount of time. This is not due to any complexity but more because of being not sure how to layout the project. What kind of functions do I need, how would the inputs from the user flow into the next parts of the program. Programming in C is not like Python where a lot can be done in one function. Every little step requires quite a lot code and thus, in most cases, should be its own function. At this point I also had an idea to include a manual entry mode for my program. Initially I did not think much about how the user would input information into the program. I just went with the thought that I would use argv and parse it for the bits I needed. However I then realised that having guided step by step inputs for those who are not familiar with CLI would be good and most importantly not difficult to implement. 





The final set of features that this program has:
- The inputs can be arguments when running the program or manual mode can be enabled that guides the user through the inputs.




PRobably should have been doing valgrind as I build and not at the very end. Quite a lot of errors.
I keep mixing up malloc(sizeof(char *string)) and malloc(strlen(char *string)). they are not the same.