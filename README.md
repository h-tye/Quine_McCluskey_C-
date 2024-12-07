# Quine_McCluskey_C-
Quine McCluskey Algorithm Implemented in C++

This C++ implementation of the Quine-McLuskey aims to find the minimum SOP form of Boolean expressions efficiently and thus provides a practical alternative to either the manual use of Karnaugh maps or the algorithm itself. It also makes the handling of functions containing more than six variables considerably easier. In this implementation, readability and reusability have been the main design drivers while trying to avoid inefficiencies inherent in the algorithm. The code is equipped to read in a boolean function of minterms from a seperate file. The simplified function will then be printed to an output file of your choosing.

# Implementation
The algorithm is carried out in the following manner:
1. The input file is processed and each line from the file is read one by one.
2. Within each line, the minterms are extracted individually and grouped into a vector.
3. The minterms are then converted into their binary equivalent representations and grouped into the first layer.
4. Within the layer, the minterms are then grouped together based off the number of ones they contain.
5. The layer is then run through, comparing each adjascent group and populating the next layer with combined terms.
6. Any terms that could not be combined further, are added to a vector of final terms.
7. Any duplicate terms are then removed from the vector.
8. With the final terms vector, the minterms that each term covers is then computed.
10. This leaves a group of simplified, prime implicants which are then formatted and exported to an output file.

# Improvements
This algorithim could be altered to work with an "infinite" number variables by altering "run_iterations()". In this function, one could create a vector of layers and just push back more layers as needed. I did not do this to avoid the headache of dealing with a vector of a vectors. Additionally, a chart to further simplify the prime implicants and essential prime implicants could be implemented.

# Demonstration
https://youtu.be/1SqfvrX3m-Q 
