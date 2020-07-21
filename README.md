# Como compilar e executar
```bash
$ g++ main.cpp -O3 -o executable
$ ./executable
```

# Multiple Traveling Thieves Problem (MTTP)

Two heuristics (GRASP, ILS) were proposed to obtain near-optimal solutions for MTTP in reasonable time.

## Descrição do Problema

O MTTP é a versão com múltiplos viajantes do problema Travelling Thief Problem (TTP), que por sua vez é dado pela combinação do Problema da Mochila como Problema do Caixeiro Viajante (NP-Difícil). 

No MTTP, são dispostos um conjunto de cidades N = {1, ..., n}, uma matriz dij que representa a distância entre cada par de cidades i,j em N. Cada item k posicionado na cidade i ́e caracterizado por um valor pik e um peso wik.
A capacidade de carregamento de itens é limitada pelo tamanho da mochila e uma taxa R é paga por cada unidade de tempo de seu aluguel. A velocidade de cada mochileiro depende da carga de seus itens carregados.

## Função Objetivo

<img src="assets/obj_function.png" />

## Resultados
Os resultados obtidos foram superiores aos dos autores Chand e Wagner[1] em todas as instâncias testadas.

## Equipe

* Gabriel Felix 
* Natan Garcias 

## Link para o relatório do trabalho
https://docs.google.com/document/d/12KOUKZ8pxM4JJjnoLRJ5sdpROEqcG36Q_CGRSA0kFos/edit?usp=sharing\


# Referências
[1] https://cs.adelaide.edu.au/~optlog/research/ttp/2016gecco-mttp.pdf
