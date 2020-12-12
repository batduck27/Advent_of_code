use std::fs::File;
use std::f64::consts::PI;
use std::io::{BufRead, BufReader, Error};
use regex::Regex;

const WAYPOINT_X_START: i64 = 10;
const WAYPOINT_Y_START: i64 = 1;

enum Action {
    North(u64),
    East(u64),
    South(u64),
    West(u64),
    LeftTurn(f64),
    RightTurn(f64),
    Forward(u64)
}

struct Ship {
    actions: Vec<Action>,
    pos_x: i64,
    pos_y: i64,
    orientation: f64,
    idx: usize,
    waypoint_x: i64,
    waypoint_y: i64
}

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn degrees_to_rad(angle: u64) -> f64 {
    (angle as f64) * (PI / 180.0)
}

fn get_actions() -> Vec<Action> {
    let input: Vec<String> = match get_input("input/day12.in") {
        Ok(content) => content,
        Err(err) => panic!("Couldn't parse input file: {:?}", err),
    };

    let re = Regex::new(r"^(\w)(\d+)$").unwrap();

    input
        .iter()
        .map(|line| {
            let caps = re.captures(line).unwrap();
            let action_str: &str = &caps[1];
            let action_val: u64 = caps[2].parse().unwrap();

            match action_str {
                "N" => Action::North(action_val),
                "E" => Action::East(action_val),
                "S" => Action::South(action_val),
                "W" => Action::West(action_val),
                "L" => Action::LeftTurn(degrees_to_rad(action_val)),
                "R" => Action::RightTurn(degrees_to_rad(action_val)),
                "F" => Action::Forward(action_val),
                _ => panic!("Invalid action: {:?}", action_str),
            }
        })
        .collect::<Vec<Action>>()
}

fn solve_part1(ship: &mut Ship) -> usize {
    ship.exec_part1_actions();
    ship.get_manhattan_dist()
}

fn solve_part2(ship: &mut Ship) -> usize {
    ship.exec_part2_actions();
    ship.get_manhattan_dist()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 12 ====================================\n");

    let mut ship = Ship {
        actions: get_actions(),
        pos_x: 0,
        pos_y: 0,
        orientation: 0.0,
        idx: 0,
        waypoint_x: 0,
        waypoint_y: 0
    };

    println!("The answer for part1 is: {}", solve_part1(&mut ship));
    println!("The answer for part2 is: {}", solve_part2(&mut ship));

    println!("\n================================================================================\n");

    Ok(())
}

impl Ship {
    fn reset(&mut self) {
        self.pos_x = 0;
        self.pos_y = 0;
        self.orientation = 0.0;
        self.idx = 0;
        self.waypoint_x = WAYPOINT_X_START;
        self.waypoint_y = WAYPOINT_Y_START;
    }

    fn get_manhattan_dist(&self) -> usize {
        (self.pos_x.abs() + self.pos_y.abs()) as usize
    }

    fn exec_part1_actions(&mut self) {
        self.reset();

        while self.idx < self.actions.len() {
            match self.actions[self.idx] {
                Action::North(steps) => self.pos_y += steps as i64,
                Action::East(steps) => self.pos_x += steps as i64,
                Action::South(steps) => self.pos_y -= steps as i64,
                Action::West(steps) => self.pos_x -= steps as i64,
                Action::LeftTurn(angle) => self.orientation += angle,
                Action::RightTurn(angle) => self.orientation -= angle,
                Action::Forward(steps) => {
                    self.pos_y += (self.orientation.sin() * (steps as f64)) as i64;
                    self.pos_x += (self.orientation.cos() * (steps as f64)) as i64;
                }
            }

            self.idx += 1;
        }
    }

    fn rotate_waypoint(&mut self, angle: f64) {
        let curr_x = self.waypoint_x as f64;
        let curr_y = self.waypoint_y as f64;

        // https://stackoverflow.com/a/2259502
        self.waypoint_x = (curr_x * angle.cos() - curr_y * angle.sin()).round() as i64;
        self.waypoint_y = (curr_x * angle.sin() + curr_y * angle.cos()).round() as i64;
    }

    fn exec_part2_actions(&mut self) {
        self.reset();

        while self.idx < self.actions.len() {
            match self.actions[self.idx] {
                Action::North(steps) => self.waypoint_y += steps as i64,
                Action::East(steps) => self.waypoint_x += steps as i64,
                Action::South(steps) => self.waypoint_y -= steps as i64,
                Action::West(steps) => self.waypoint_x -= steps as i64,
                Action::LeftTurn(angle) => self.rotate_waypoint(angle),
                Action::RightTurn(angle) => self.rotate_waypoint(-angle),
                Action::Forward(steps) => {
                    self.pos_x += self.waypoint_x * (steps as i64);
                    self.pos_y += self.waypoint_y * (steps as i64);
                }
            }

            self.idx += 1;
        }
    }
}