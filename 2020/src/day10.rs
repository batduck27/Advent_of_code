use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader};

const MAX_DIFF: u64 = 3;

fn get_input(filename: &str) -> Vec<u64> {
    BufReader::new(File::open(filename).unwrap())
        .lines()
        .map(|line| line.unwrap().parse().unwrap())
        .collect()
}

fn solve_part1(adapters: &Vec<u64>) -> u64 {
    let mut diffs_cnt: HashMap<u64, u64> = HashMap::new();

    for i in 1..adapters.len() {
        let diff = adapters[i] - adapters[i - 1];
        *diffs_cnt.entry(diff).or_insert(0) += 1;
    }

    diffs_cnt.get(&1).unwrap_or(&0) * diffs_cnt.get(&3).unwrap_or(&0)
}

fn solve_part2(adapters: &Vec<u64>) -> u64 {
    let mut dp: Vec<u64> = vec![0; adapters.len() - 1];

    dp[0] = 1;

    for i in 1..dp.len() {
        let mut j: i32 = (i - 1) as i32;

        while j >= 0 && adapters[i] - adapters[j as usize] <= MAX_DIFF {
            dp[i] += dp[j as usize];
            j -= 1;
        }
    }

    *dp.last().unwrap()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 10 ====================================\n");

    let mut adapters = get_input("input/day10.in");

    adapters.push(0);
    adapters.sort();
    adapters.push(adapters.last().unwrap() + MAX_DIFF);


    println!("The answer for part1 is: {}", solve_part1(&adapters));
    println!("The answer for part2 is: {}", solve_part2(&adapters));

    println!("\n================================================================================\n");

    Ok(())
}
