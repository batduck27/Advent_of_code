'''
Solution for the 11th day of Advent of Code 2021 (https://adventofcode.com/2021/day/11).
'''

import queue

class OctopusesSimulator:
    ''' Simulates the behavior of the octopuses. '''

    # Increments to navigate vertically/horizontally in the grid.
    VERT = [-1, -1, 0, 1, 1, 1, 0, -1]
    ORIZ = [0, 1, 1, 1, 0, -1, -1, -1]

    # Maximum allowed energy
    MAX_ENERGY = 9

    def __init__(self, grid):
        ''' Constructor.
        :param grid: Matrix of ints that represents the initial energy levels of octopuses.
        '''
        self.flash_events = None
        self.octopuses = {}

        for i, _ in enumerate(grid):
            for j, _ in enumerate(grid[i]):
                self.octopuses[(i, j)] = grid[i][j]

    def __increase_energy_level(self, pos):
        ''' Increases the energy level of an octopus by one level.
        :param pos: Position of the octopus in grid.
        :return: True if the energy level becomes greater than the maximum energy level, False
                 otherwise
        '''
        flash = False

        self.octopuses[pos] = (self.octopuses[pos] + 1) % (self.MAX_ENERGY + 1)
        if self.octopuses[pos] == 0:
            flash = True
            self.flash_events.put_nowait(pos)

        return flash

    def __simulate_step(self):
        ''' Simulate a iteration.
        :return: Number of flashes that happened in this iteration.
        '''
        flashes = 0
        self.flash_events = queue.Queue()

        for pos in self.octopuses:
            flashes += self.__increase_energy_level(pos)

        while not self.flash_events.empty():
            i, j = self.flash_events.get_nowait()

            for k in range(len(self.VERT)):
                new_i = i + self.VERT[k]
                new_j = j + self.ORIZ[k]
                new_pos = (new_i, new_j)

                if new_pos in self.octopuses and self.octopuses[new_pos] != 0:
                    flashes += self.__increase_energy_level(new_pos)

        return flashes

    def simulate(self, target_iterations):
        ''' Simulate a number of iterations.
        :param target_iterations: Number of iterations to simulate.
        :return: Number of flashes that happened during the simulation.
        '''
        flashes = 0

        for _ in range(target_iterations):
            flashes += self.__simulate_step()

        return flashes

    def simulate_until_synchronize(self):
        ''' Simulate the octopuses behavior until all the grid synchronizes.
        :return: First step during which all the octopuses flash.
        '''
        steps = 0

        while True:
            steps += 1

            if self.__simulate_step() == len(self.octopuses):
                break

        return steps


def main():
    ''' Parse the input and solve both parts. '''
    with open('data.in', 'r', encoding='utf-8') as fhd:
        grid = [[int(x) for x in line] for line in fhd.read().splitlines()]

    print(f'The solution for part1 is: {OctopusesSimulator(grid).simulate(100)}')
    print(f'The solution for part2 is: {OctopusesSimulator(grid).simulate_until_synchronize()}')

# Entry point
if __name__ == "__main__":
    main()
