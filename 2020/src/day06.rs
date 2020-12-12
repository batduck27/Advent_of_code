use std::fs;
use std::collections::HashSet;
use std::iter::FromIterator;

fn get_input(filename: &str) -> Vec<Vec<String>> {
    let input_as_string = fs::read_to_string(filename).unwrap();

    input_as_string
        .split("\n\n")
        .map(|chunk| chunk.split("\n").map(|answer| answer.to_string()).collect())
        .collect()
}

fn solve_part1(answers: &Vec<Vec<String>>) -> usize {
    let mut ans: usize = 0;

    for group in answers {
        let sets: Vec<HashSet<char>> = group
            .into_iter()
            .map(|answer| HashSet::from_iter(answer.chars()))
            .collect();

        ans += sets.iter().fold(HashSet::new(), |acc, set| acc.union(&set).copied().collect()).len();
    }

    ans
}

fn solve_part2(answers: &Vec<Vec<String>>) -> usize {
    let mut ans: usize = 0;

    for group in answers {
        let sets: Vec<HashSet<char>> = group
            .into_iter()
            .map(|answer| HashSet::from_iter(answer.chars()))
            .collect();

        ans += sets
            .iter()
            .skip(1)
            .fold(sets[0].clone(), |acc, set| acc.intersection(&set).copied().collect())
            .len();
    }

    ans
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 04 ====================================\n");

    let answers = get_input("input/day06.in");

    println!("The answer for part1 is: {}", solve_part1(&answers));
    println!("The answer for part2 is: {}", solve_part2(&answers));

    println!("\n================================================================================\n");

    Ok(())
}
