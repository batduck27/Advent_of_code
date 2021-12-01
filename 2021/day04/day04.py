'''
Solution for the 4th day of Advent of Code 2021 (https://adventofcode.com/2021/day/4).
'''

import itertools

#pylint: disable=too-many-locals
def simulate_bingo_game(drawn_numbers, bingo_boards):
    ''' Simulate the a bingo game using the extracted numbers and the configuration of
    all bingo boards.
    :param drawn_numbers: The order in which the numbers are extracted.
    :param bindo_boards: The configurations of the bingo boards.
    :return: (the winner score -1st part-, the loser score -2nd part-)
    '''
    min_draws = len(drawn_numbers) + 1
    winner_score = -1

    max_draws = 0
    loser_score = -1

    for board in bingo_boards:
        cols_num = len(board[0])
        rows_num = len(board)
        marked_on_rows = [cols_num for _ in range(rows_num)]
        marked_on_cols = [rows_num for _ in range(cols_num)]

        flatten_board = list(itertools.chain.from_iterable(board))
        remaining_nums = set(flatten_board)

        for step, num in enumerate(drawn_numbers):
            try:
                idx = flatten_board.index(num)

                row = idx // cols_num
                col = idx % rows_num

                marked_on_cols[col] -= 1
                marked_on_rows[row] -= 1
                remaining_nums.remove(num)

                if marked_on_rows[row] <= 0 or marked_on_cols[col] <= 0:
                    if step < min_draws:
                        min_draws = step
                        winner_score = num * sum(remaining_nums)

                    if step > max_draws:
                        max_draws = step
                        loser_score = num * sum(remaining_nums)

                    break
            except ValueError:
                pass

    return (winner_score, loser_score)

def main():
    ''' Parse the input and solve both parts. '''
    with open('data.in', 'r', encoding='utf-8') as fhd:
        drawn_numbers = [int(x) for x in fhd.readline().strip().split(',')]
        bingo_boards = [[[int(x) for x in row.strip().split()] for row in matrix.split('\n')]
                        for matrix in fhd.read().strip().split('\n\n')]

    winner_score, loser_score = simulate_bingo_game(drawn_numbers, bingo_boards)

    print(f'The solution for part1 is: {winner_score}')
    print(f'The solution for part2 is: {loser_score}')

# Entry point
if __name__ == "__main__":
    main()
