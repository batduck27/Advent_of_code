'''
Solution for the 1st day of Advent of Code 2021 (https://adventofcode.com/2021/day/1).
'''

# Windows size used for part2.
WINDOW_SIZE = 3

def count_increasing(sweep_report):
    ''' Count how many times the depth increases from the previous measurement.
    :param sweep_report: Array representing the recorded depths.
    :return: Number of increasing measurements.
    '''
    cnt = 0

    for i in range(1, len(sweep_report)):
        if sweep_report[i] > sweep_report[i - 1]:
            cnt += 1

    return cnt

def solve_part1(sweep_report):
    ''' Solve the first part by counting the number of increasing measurements.
    :param sweep_report: Array representing the recorded depths.
    :return: Number of increasing measurements.
    '''
    return count_increasing(sweep_report)

def solve_part2(sweep_report):
    ''' Solve the second part by counting the number of increasing three-measurement window.
    :param sweep_report: Array representing the recorded depths.
    :return: Number of increasing measurements.
    '''
    windowed_depths = []

    for i in range(len(sweep_report) - WINDOW_SIZE + 1):
        windowed_depths.append(sum(sweep_report[i:i + WINDOW_SIZE]))

    return count_increasing(windowed_depths)

def main():
    ''' Parse the input and solve both parts. '''
    with open('data.in', 'r', encoding='utf-8') as fhd:
        sweep_report = [int(x) for x in fhd.read().splitlines()]

    print(f'The solution for part1 is: {solve_part1(sweep_report)}')
    print(f'The solution for part2 is: {solve_part2(sweep_report)}')

# Entry point
if __name__ == "__main__":
    main()
