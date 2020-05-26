# Compiling & Running
```bash
$ g++ heuristics_name.cpp -O3 -fopenmp -o heurisitics_name.out
$ heuristics_name.out < path/to/instance

# Multiple Traveling Thieves Problem (MTTP)

Two heuristics (GRASP, ILS) were proposed to obtain near-optimal solutions for MTTP in reasonable time.

## Problem Description

O MTTP é a versão com múltiplos viajantes do problema Travelling Thief Problem (TTP), que por sua vez é dado pela combinação do Problema da Mochila como Problema do Caixeiro Viajante (NP-Difícil). 

No MTTP, são dispostos um conjunto de cidades N = {1, ..., n}, uma matriz dij que representa a distância entre cada par de cidades i,j em N. Cada item k posicionado na cidade i ́e caracterizado por um valor pik e um peso wik.
A capacidade de carregamento de itens é limitada pelo tamanho da mochila e uma taxa R é paga por cada unidade de tempo de seu aluguel. A velocidade de cada mochileiro depende da carga de seus itens carregados.

## Objective Function

<img src="readme_attributes/obj_function.png" />

## Hipothetical Example

## Results
The obtained results were superior than the ones obtained by Chand e Wagner[1].

## Official Contributors

* Gabriel Felix 
* Natan Garcias 

## Full report link for more details
https://docs.google.com/document/d/12KOUKZ8pxM4JJjnoLRJ5sdpROEqcG36Q_CGRSA0kFos/edit?usp=sharing\


# References
[1] https://cs.adelaide.edu.au/~optlog/research/ttp/2016gecco-mttp.pdf
