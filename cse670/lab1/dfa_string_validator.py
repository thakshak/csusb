'''
                      *****************************
                         CS570/670 -- Spring 2017

                                 LAB1

                         Instructor: Ernesto Gomez
                      ******************************
DFAs are specified by a set of states (among them one starting and one 
or more accepting), and transitions between pairs of states which are 
labeled by a symbol of some alphabet. DFAs are typically given in one 
of two formats: 
(1) a transition diagram (that is, a directed graph), and
(2) a transition table. 
Both representations are equivalant to each other.  

As an example of a DFA, take a look in RED dragon book on p. 121, Fig. 
3.29. You see a transition diagram. On p. 120, you find the equivalent 
transition table. 

PURPLE dragon book: pg 148-149. figs 3.28

EXERCISE: Write a program that simulates the above DFA 
	in a language of your choice, but limited to something that
	could run on the machines in jb358
	
	  Accepts L=(a|b)*abb

	IT IS STRONGLY RECOMMENDED THAT YOU USE A TABLE DRIVEN
	APPROACH TO WRITING YOUR DFA.
	  
          A good program will be one that is easily modified
          to handle other transition diagrams.

          The input to the DFA should be a string; in the course
          of processing this string character-by-character, the
          DFA will undergo the specified state transitions. The
	  DFA accepts the string if it is an accepting state
	  when it has consumed its input; otherwise it rejects
	  the string.

DELIVERABLES:
	  Program source code, with internal documentation
	  Test results : use strings that are in L, and strings
	  not in L.

With thanks to: Kerstin Voigt
'''

'''
DFA:
		a b
	  0 1 0
	  1 1 2
	  2 1 3
	  3 1 0

Description of Input File(file1.dfa):

1st line states in dfa - 0 1 2 3
2nd line alphabets in language - a b
3rd line initial_state - 0
4th line final_state - 3
followed by (number_of_states) lines with transitions.
1 0
1 2
1 3
1 0 
'''

#=======================Program Starts here======================#

f = open('file1.dfa', 'r')
states=f.readline().strip().split()
number_of_states=len(states)
alphabets_in_language=f.readline().strip().split()
initial_state=f.readline().strip()
final_states=f.readline().strip().split()
data=[]
for i in xrange(number_of_states):
	data.append(f.readline().strip().split())

transitions = {}
for x in xrange(number_of_states):
	for y in xrange(len(alphabets_in_language)):
		transitions[(states[x],alphabets_in_language[y])] = data[x][y]
		
strng_to_check = raw_input("enter string : ");

current_state = initial_state

for alphabet in strng_to_check:
	current_state = transitions[(current_state,alphabet)]
  
if current_state in final_states:
	print "String Accepted"
else:
	print "String Rejected"
  
