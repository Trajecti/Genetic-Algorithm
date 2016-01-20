# Genetic-Algorithm

This program written in C++ utilizes a genetic algorithm to solve the Travelling Salesman problem (an NP hard problem). As a result, it would be difficult, lengthy and compuatationally expensive to produce an objectively correct answer to a probelm whose space grows exponentially.

A genetic algoritm randomly generates path strings of cities and calculates their overall length. By applying the PMX crossover algorithm, new children of sucessful "genes" are created and then the best of those are slelected for a new generation. The result is an algorithm that quickly reaches a near optimal , if not optimal, length.  
