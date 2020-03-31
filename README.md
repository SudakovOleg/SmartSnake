# SmartSnake
The standard game "Snake" in which the game is played not by a person, but by a computer. I'm use a simple neural network and a genetic algorithm for its development.

This program has several settings that regulate the operation of the genetic algorithm. There are also simple configuration for the architecture of the neural network.

The genetic algorithm settings have the following structure:  
  1) The setting of mutation:   
    a)Gene mutation - сhanges the random weight in the weight matrix, in the specified range.  
    b)The shift of chromosomes - shifts weights within a column of the weight matrix.  
    с)Mixed (Including both modes).  
  2) Generation settings:  
    a) Setting the number of individuals per generation.  
    b) Setting the number of generations.  
    c) Setting the number of individuals to cross. 
