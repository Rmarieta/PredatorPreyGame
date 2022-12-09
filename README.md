# Implementation of a Predator Prey Game in C++

In this implementation, an environment is run and displays 3 different types of objects :

- wolves
- sheeps
- grass

Wolves can eat sheeps, which reduces the population of sheeps, and the sheeps can eat grass and can also mate with one another, therefore increasing their population.

Objects randomly pop on the environment with a certain probability at any given time, but can also be added anywhere in the environment window by pressing the associated key : 'W' to add a wolf, 'S' to add a sheep and 'G' to add a patch of grass. The object is added where the cursor is pointing at.

There also is an option to run it with very large starting populations of each in order to analyse how each population is going to reach stability with regards to their respective predators and preys. A plot displays the evolution of each population as a function of time steps all along the simulation.
