Christopher Hanly
CS.3360.253 Computing Systems Fundamentals
Homework 5
Professor: Mina Guirguis
04/22/2024

In Homework 5, we expand on Homework 4's Discrete Time Event Simulator.
Now we present a Discrete Time Event Simulator that offers two designs:
1. The "Grocery Store" design that uses an individual ready queue for each processor
2. The "Airport" design that uses a central ready queue for all processors

Based on the console arguments provided, one of either scenario will be ran by the program.
Once complete, metrics will be displayed to the console and exported to either "cmh171s1.txt"
or "cmh171s2.txt", depending on the scenario selected.

Furthermore, certain parts of this implementation are designed to handle a variable number of processors.
However, it appears that fully implementing "n" processors is extraneous to design specifications.
Thus, the design is implemented specifically for 4 processors and will exhibit problems if any other
number of processors is used. 

Finally, the repo is complete with multirun1.sh and multirun2.sh which have been written to support
iterating through the requested console argument sets. They will write to the aforementioned .txt files.

1. Compile with makefile in directory ($ > make)
2. Call program with four arguments e.g. ($ > ./hw5 50 0.02 1 4)
3. Single run output will result in console.
4. Note: The following writes to "cmh171s1.txt" or "cmh171s2.txt" in the current directory
	For multiple runs, call one of the multirun_.sh scripts
	You may need to grant yourself execution permission eg "chmod +x multirun1.sh"
5. Report .pdf in folder