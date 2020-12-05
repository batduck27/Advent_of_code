use std::fs::File;
use std::io::{BufRead, BufReader};

const TREE : char = '#';

fn get_input(filename: &str) -> Vec<Vec<char>> {
    BufReader::new(File::open(filename).unwrap())
        .lines()
        .map(|line| line.unwrap().chars().collect::<Vec<char>>())
        .collect::<Vec<Vec<char>>>()
}

fn get_slope_result(grid: &Vec<Vec<char>>, y_step: usize, x_step: usize) -> usize {
    let mut y = 0;
    let mut x = 0;
    let mut ans = 0;

    while y < grid.len() {
        if grid[y][x] == TREE {
            ans += 1;
        }

        x = (x + x_step) % grid[y].len();
        y += y_step;
    }

    ans
}

fn solve_part1(grid: &Vec<Vec<char>>) -> usize {
    get_slope_result(grid, 1, 3)
}

fn solve_part2(grid: &Vec<Vec<char>>) -> usize {
    get_slope_result(grid, 1, 1) * get_slope_result(grid, 1, 3) *
    get_slope_result(grid, 1, 5) * get_slope_result(grid, 1, 7) *
    get_slope_result(grid, 2, 1)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 03 ====================================\n");

    let grid : Vec<Vec<char>> = get_input("input/day03.in");

    println!("The answer for part1 is: {}", solve_part1(&grid));
    println!("The answer for part2 is: {}", solve_part2(&grid));

    println!("\n================================================================================\n");

    Ok(())
}
