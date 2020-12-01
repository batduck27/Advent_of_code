use std::collections::HashSet;
use std::fs::File;
use std::io::{BufRead, BufReader, Error, ErrorKind};
use std::iter::FromIterator;

const DESIRED_SUM : i64 = 2020;

fn get_input(filename: &str) -> Result<Vec<i64>, Error> {
    let reader = BufReader::new(File::open(filename)?);

    reader.lines()
        .map(|line| line.and_then(|v| v.parse().map_err(|e| Error::new(ErrorKind::InvalidData, e))))
        .collect()
}

fn solve_part1(numbers : &Vec<i64>) -> Result<i64, Error> {
    let numbers_set : HashSet<i64> = HashSet::from_iter(numbers.iter().cloned());

    for num in &numbers_set {
        let diff = DESIRED_SUM - num;

        if numbers_set.contains(&diff) {
            return Ok(num * diff);
        }
    }

    return Err(Error::new(ErrorKind::InvalidData, "Couldn't find two numbers that add up to 2020"));
}

fn solve_part2(numbers : &mut Vec<i64>) -> Result<i64, Error> {
    numbers.sort();

    for i in 0..numbers.len() {
        let mut left = i + 1;
        let mut right = numbers.len() - 1;

        while left < right {
            let sum = numbers[i] + numbers[left] + numbers[right];

            if sum == DESIRED_SUM {
                return Ok(numbers[i] * numbers[left] * numbers[right]);
            }

            if sum < DESIRED_SUM {
                left += 1;
            } else {
                right -= 1;
            }
        }
    }

    return Err(Error::new(ErrorKind::InvalidData, "Couldn't find three numbers that add up to 2020"));
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 01 ====================================\n");

    let mut numbers : Vec<i64> = match get_input("input/day01.in") {
        Ok(content) => content,
        Err(err) => return Err(err.into()),
    };

    println!("The answer for part1 is: {}", solve_part1(&numbers).unwrap());
    println!("The answer for part2 is: {}", solve_part2(&mut numbers).unwrap());

    println!("\n================================================================================\n");

    Ok(())
}
