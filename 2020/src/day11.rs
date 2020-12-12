use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::ops::*;

const PART1_THRESEHOLD: usize = 4;
const PART2_THRESEHOLD: usize = 5;

const EMPTY_SEAT: char = 'L';
const TAKEN_SEAT: char = '#';
const FLOOR_TILE: char = '.';

const DIRS: [Position; 8] = [Position(-1, 0), Position(-1, 1), Position(0, 1), Position(1, 1),
                             Position(1, 0), Position(1, -1), Position(0, -1), Position(-1, -1)];

type AdjacentSeats = HashMap<Position, Vec<Position>>;
type SeatState = HashMap<Position, char>;

#[derive(Hash, Eq, PartialEq, Copy, Clone, Debug)]
struct Position(i32, i32);

fn get_input(filename: &str) -> Vec<Vec<char>> {
    BufReader::new(File::open(filename).unwrap())
        .lines()
        .map(|line| line.unwrap().chars().collect::<Vec<char>>())
        .collect::<Vec<Vec<char>>>()
}

fn part1_get_adjacent_seats(seats: &SeatState) -> AdjacentSeats {
    let mut adjacent_seats: AdjacentSeats = HashMap::new();

    for seat_pos in seats.keys() {
        for dir in &DIRS {
            let neigh_pos = *seat_pos + *dir;
            if seats.contains_key(&neigh_pos) {
                adjacent_seats.entry(*seat_pos).or_insert(Vec::new()).push(neigh_pos);
            }
        }
    }

    adjacent_seats
}

fn part2_get_adjacent_seats(seats: &SeatState) -> HashMap<Position, Vec<Position>> {
    let mut adjacent_seats: HashMap<Position, Vec<Position>> = HashMap::new();
    let mut max_x: i32 = 0;
    let mut max_y: i32 = 0;

    for Position(pos_y, pos_x) in seats.keys() {
        max_x = std::cmp::max(max_x, *pos_x);
        max_y = std::cmp::max(max_y, *pos_y);
    }

    for seat_pos in seats.keys() {
        for dir in &DIRS {
            let mut neigh_pos = *seat_pos + *dir;

            while neigh_pos.is_valid(max_y, max_x) && !seats.contains_key(&neigh_pos) {
                neigh_pos = neigh_pos + *dir;
            }

            if seats.contains_key(&neigh_pos) {
                adjacent_seats.entry(*seat_pos).or_insert(Vec::new()).push(neigh_pos);
            }
        }
    }

    adjacent_seats
}

fn simulate(grid: &Vec<Vec<char>>, thresehold: usize, get_adjacent: &dyn Fn(&SeatState) -> AdjacentSeats) -> usize {
    let mut seats: SeatState = HashMap::new();
    let mut seat_change: bool = true;

    for i in 0..grid.len() {
        for j in 0..grid[i].len() {
            if grid[i][j] != FLOOR_TILE {
                seats.insert(Position(i as i32, j as i32), grid[i][j]);
            }
        }
    }

    let adjacent_seats = get_adjacent(&seats);

    while seat_change {
        let mut new_seats: SeatState = HashMap::new();
        seat_change = false;

        for (pos, state) in seats.clone().iter() {
            let occupied_neighs = adjacent_seats.get(pos).unwrap_or(&vec![])
                .into_iter()
                .filter(|neigh_pos| *(seats.get(neigh_pos).unwrap_or(&EMPTY_SEAT)) == TAKEN_SEAT)
                .count();

            if *state == EMPTY_SEAT && occupied_neighs == 0 {
                new_seats.insert(*pos, TAKEN_SEAT);
                seat_change = true;
            } else if *state == TAKEN_SEAT && occupied_neighs >= thresehold {
                new_seats.insert(*pos, EMPTY_SEAT);
                seat_change = true;
            } else {
                new_seats.insert(*pos, *state);
            }
        }

        seats = new_seats;
    }

    seats.values().filter(|seat_state| **seat_state == TAKEN_SEAT).count()
}

fn solve_part1(grid: &Vec<Vec<char>>) -> usize {
    simulate(grid, PART1_THRESEHOLD, &part1_get_adjacent_seats)
}

fn solve_part2(grid: &Vec<Vec<char>>) -> usize {
    simulate(grid, PART2_THRESEHOLD, &part2_get_adjacent_seats)
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 11 ====================================\n");

    let grid = get_input("input/day11.in");

    println!("The answer for part1 is: {}", solve_part1(&grid));
    println!("The answer for part2 is: {}", solve_part2(&grid));

    println!("\n================================================================================\n");

    Ok(())
}

impl Add for Position {
    type Output = Position;

    fn add(self, other: Position) -> Position {
        Position(self.0 + other.0, self.1 + other.1)
    }
}

impl Position {
    fn is_valid(&self, max_y: i32, max_x: i32) -> bool {
        self.0 >= 0 && self.1 >= 0 && self.0 < max_y && self.1 < max_x
    }
}