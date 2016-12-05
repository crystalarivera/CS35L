#!/usr/bin/python 
#Tells the shell which interpreter to use

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys #Import statements, similar to include statements
from optparse import OptionParser #Import OptionParser class from optparse module

class randline: #The beginning of the class statement: randline
    def __init__(self, filename): #The constructor
        f = open(filename, 'r') #Creates a file handle
        self.lines = f.readlines() #Reads into file a list of strings called lines
        f.close() #Close the file

    def chooseline(self): #The beginning of a function belonging to randline
        return random.choice(self.lines) #Randomly select a number between 0 and the size of lines and returns the line corresponding to the randomly selected number

def main(): #The beginning of the main function
    version_msg = "%prog 2.0" #version message
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE.""" #usage message

    parser = OptionParser(version=version_msg,
                          usage=usage_msg) #creates Option Parser instance
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)") #Start defining options,  action "store" tells optparse to take next  argument and store to the right destination which is "numlines".  Set the default value of "numlines" to 1 and help message.
    options, args = parser.parse_args(sys.argv[1:]) 
#options: an object containing all options args.
#args: list of positional args leftover after parsing options

    try: #Try block
        numlines = int(options.numlines) #get numlines from options and convert to integer
    except: #Exception handling
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines)) #error message if numlines is not integer type, replace {0} w/ input
    if numlines < 0: #If numlines is negative
        parser.error("negative count: {0}".
                     format(numlines)) #error message
    if len(args) != 1: #If length of args is not 1 (no file name or more than one file name)
        parser.error("wrong number of operands") #error message
    input_file = args[0] #assign the first and only argument to variable input_file

    try: #Try block
        generator = randline(input_file) #instantiate randline object with parameter input_file
        for index in range(numlines): #for loop, iterate from 0 to numlines 
            sys.stdout.write(generator.chooseline()) #print the randomly chosen line
    except IOError as xxx_todo_changeme: #Exception handling
        (errno, strerror) = xxx_todo_changeme.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror)) #error message in the format I/O error (errno):strerror

if __name__ == "__main__":
    main() #In order to make the Python file a standalone program
