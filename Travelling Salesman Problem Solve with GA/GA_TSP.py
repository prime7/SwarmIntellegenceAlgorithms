from random import sample
from random import random
from random import uniform
from random import shuffle
from math import sqrt
from time import time
from itertools import permutations


class NodesLeastDistanceGA:
    def __init__(self, parent, side, verbose=True):
        self._parent = parent
        self._side = side

        self._mutate_rate = 0.07
        self._population_size = 60 if len(parent) > 10 else 10
        self._new_generation_size = self._population_size*2
        self._rounds = 300
        self._genlen = len(parent)
        self._verbose = verbose
        self._cached_distances = {}
        self._cached_fitness = {}
        

    def algorithm(self):
        population = self.generate_population()
        fitest = min(population, key=self.fitness)

        total_time = time()
        for r in range(self._rounds):
            new_pop = []
            while len(new_pop) < self._new_generation_size:
                father = self.select(population)
                mother = self.select(population)
                child = self.crossover(father, mother)
                if child not in new_pop:
                    new_pop += [child]
                    continue
                for i in range(len(new_pop)):
                    if random() < self._mutate_rate:
                        new_pop[i] = self.mutate(new_pop[i])

            new_fittest = min(population, key=self.fitness)
            if self.fitness(fitest) > self.fitness(new_fittest):
                fitest = new_fittest
            if r % 50 == 0:
                print(r, self.fitness(min(population, key=self.fitness)))

            population = self.selection(new_pop)
            if fitest not in population:
                population += [fitest]

        self.result(population, fitest, total_time)
        

    def result(self, population, fitest, total_time):

        print("Execution Time: {:.3f}s".format(time() - total_time))
        print("Best path found: {}, fitness: {:.3f}".format(fitest, self.fitness(fitest)))

    def selection(self, new_pop):
        shuffle(new_pop)
        pop = []
        for _ in range(self._population_size):
            survivor = self.select(new_pop)
            new_pop.remove(survivor)
            pop += [survivor]
        return pop

    def select(self, pop):
        pop_total_fit = sum(1.0 / self.fitness(p) for p in pop)
        limit = uniform(0.0, pop_total_fit)
        c = 0
        for p in pop:
            c += 1 / self._cached_fitness[hash(tuple(p))]
            if c > limit:
                return p

    def fitness(self, child):
        h = hash(tuple(child))
        if h in self._cached_fitness.keys():
            return self._cached_fitness[h]

        distance = 0
        for i in range(len(child)-1):
            distance += self.point_distance(child[i], child[i+1])
        self._cached_fitness[h] = distance
        return distance

    @staticmethod 
    def crossover(father, mother):    
        child = [None]*len(father)
        rate = 0.5
        for gen in father:
            parent, other_parent = (father, mother) if random() > rate else (mother, father)
            key = None
            for key, value in enumerate(parent):
                if value == gen:
                    break
            if not child[key]:
                child[key] = gen
                continue
            for key, value in enumerate(other_parent):
                if value == gen:
                    break
            if not child[key]:
                child[key] = gen
                continue
            for key, value in enumerate(child):
                if not value:
                    child[key] = gen
                    break
        return child

    @staticmethod
    def mutate(child):        
        i1, i2 = sample(range(1, len(child)-1), 2)
        child[i1], child[i2] = child[i2], child[i1]
        return child

    def point_distance(self, p1, p2):
        nodes = hash((p1, p2))
        if nodes in self._cached_distances.keys():
            return self._cached_distances[nodes]
        
        d = sqrt((p2[0]-p1[0])**2 + (p2[1]-p1[1])**2)
        self._cached_distances[nodes] = d
        return d

    def generate_population(self):
        pop = [self._parent[:1]+sample(self._parent[1:-1], len(self._parent)-2)+self._parent[-1:]
               for _ in range(self._population_size)]
        for p in pop:
            h = hash(tuple(p))
            self._cached_fitness[h] = self.fitness(p)
        return pop


    def correct_ans(self, nodes):
        if len(nodes) > 10:
            print("Not a good idea.")
            raise Exception
        start = time()
        best = nodes
        for path in permutations(nodes[1:-1]):
            path = nodes[:1]+list(path)+nodes[-1:]
            if self.fitness(best) > self.fitness(path):
                best = path
        print("Correct ans: {}: fitness: {:.3f}, solutions: {}".format(str(best), 
                    self.fitness(best), len(list(permutations(nodes)))))
        print("Bruteforce Timer: {:.3f}".format(time()-start))
        
def main():
    nodes = [(60,200),(180,200),(80,180),(140,180),(20,160),(100,160),(13, 2), (1, 12), (12, 5)#,(19, 6)],(2, 10),(15, 15),(5, 11),(17, 9),(10, 18),(17, 5),(13, 12),(1, 17),(2, 6),(7, 16),(19, 2),(3, 7),(10, 9),(5, 19)]
    nodes + = nodes[:1]
    ga = NodesLeastDistanceGA(nodes,20)
    ga.correct_ans(nodes)
    ga.algorithm()


if __name__ == '__main__':
    main()