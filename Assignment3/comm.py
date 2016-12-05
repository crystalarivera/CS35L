#!/usr/bin/python 
#Tells the shell which interpreter to use

import random, sys #Import statements, similar to include statements
from optparse import OptionParser #Import OptionParser class from optparse module

class listfile: #The beginning of the class statement: randline
    def __init__(self, filename): #The constructor
        if filename == "-":
            self.lines = sys.stdin.readlines()
        else:
            f = open(filename, 'r') #Creates a file handle
            #Reads into file a list of strings called lines
            self.lines = f.readlines()
            f.close() #Close the file

        #checks to see if file is empty and if last item in list is missing \n
        if len(self.lines) != 0:
            temp = self.lines[-1]
            NEW_LINE = "\n"
            if NEW_LINE not in temp:
                temp += NEW_LINE
                self.lines[-1] = temp

        ''' unneded function
    def chooseline(self): #The beginning of a function belonging to randline
        return random.choice(self.lines) #Randomly select a number between 0 and the size of lines and returns the line corresponding to the randomly selected number
        '''

def compare(fileone, filetwo, show1, show2, show3, unsorted):
    i, j, k = 0, 0, 0
    len_file1, len_file2 = len(fileone), len(filetwo)

    #-u algorithm 
    if (unsorted):
        while i < len_file1:
                #once again, the conditional allows the loops to run just past the condition, so i != len_file1 must be here........
            if j != len_file2:
                if fileone[i] == filetwo[j]:
                    if (show3):
                        if (show1):
                            sys.stdout.write('\t')
                        if (show2):
                            sys.stdout.write('\t')
                        sys.stdout.write(fileone[i])
                        temp = filetwo[j]
                        filetwo.remove(filetwo[j])
                        filetwo.insert(0, temp)
                    i += 1
                    k += 1
                    j = k
                else:
                    j += 1      
            else:
                if (show1):
                    sys.stdout.write(fileone[i])
                j = k
                i += 1
                        
        #write the rest of the second column
        if (show2):
            while j < len_file2:
                if (show1):
                    sys.stdout.write('\t')    
                sys.stdout.write(filetwo[j])
                j += 1
            
        return
        
    #the sorted algorithm
    while i < len_file1 or j < len_file2:

        #note that even though while has its conditions, if fileone[i] == "" still produced problems
        if j == len_file2 or (i != len_file1 and fileone[i] < filetwo[j]):
            if (show1):
                sys.stdout.write(fileone[i])
            i += 1
        elif i == len_file1 or fileone[i] > filetwo[j]:
            if (show2):
                if (show1):
                    sys.stdout.write('\t')
                sys.stdout.write(filetwo[j])
            j += 1
        else:
            if (show3):
                if (show1):
                    sys.stdout.write('\t')
                if (show2):
                    sys.stdout.write('\t')
                sys.stdout.write(fileone[i])
            i += 1
            j += 1
                   
    return
    

def main(): #The beginning of the main function
    version_msg = "%prog 1.0" #version message
    usage_msg = """%prog [OPTION]... FILE1 FILE2

Compares lines between FILE1 and FILE2.""" #usage message

    parser = OptionParser(version=version_msg,
                          usage=usage_msg) #creates Option Parser instance
    parser.add_option("-1", "--hidecolumn1",
                      action="store_false", dest="columnone", default=True,
                      help="hides column that outputs lines unique to FILE1") #Start defining options,  action "store" tells optparse to take next  argument and store to the right destination which is "numlines".  Set the default value of "numlines" to 1 and help message.
    parser.add_option("-2", "--hidecolumn2",
                      action="store_false", dest="columntwo", default=True,
                      help="hides column that outputs lines unique to FILE2")
    parser.add_option("-3", "--hidecolumn3",
                      action="store_false", dest="columnthree", default=True,
                      help="hides column that outputs lines that appear in both files")
    parser.add_option("-u", "--unsorted",
                      action="store_true", dest="unsorted", default=False,
                      help="Handles the case that one or both files are unsorted")
    options, args = parser.parse_args(sys.argv[1:])
#options: an object containing all options args. now holds one, two, three, and unsorted boolean values
#args: list of the non option inputs

    try: #Try block
        showone = bool(options.columnone)
        showtwo = bool(options.columntwo)
        showthree = bool(options.columnthree)
        unsorted = bool(options.unsorted)
        #you could also just access them with options.columnone but this works
    except: #Exception handling
        ''' unnneeded exceptions
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines)) #error message if numlines is not integer type, replace {0} w/ input
    if numlines < 0: #If numlines is negative
        parser.error("negative count: {0}".
                     format(numlines)) #error message
                     '''
    if len(args) != 2: #If length of args is not 1 (no file name or more than one file name)
        parser.error("wrong number of operands") #error message    
        
    FileOne = args[0] #assign the first argument to variable input_file
    FileTwo = args[1]
    ListOne = listfile(FileOne)
    ListTwo = listfile(FileTwo)
    listone = ListOne.lines
    listtwo = ListTwo.lines #kinda annoying you need to access args, put in a temp variable, then finally instantiate the class
    
    try: #Try block
        compare(listone, listtwo, showone, showtwo, showthree, unsorted)
        '''
        for index in range(numlines): #for loop, iterate from 0 to numlines 
            sys.stdout.write(generator.chooseline()) #print the randomly chosen line
            '''
    except IOError as xxx_todo_changeme: #Exception handling
        (errno, strerror) = xxx_todo_changeme.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror)) #error message in the format I/O error (errno):strerror

if __name__ == "__main__":
    main() #In order to make the Python file a standalone program
