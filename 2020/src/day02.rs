use std::fs::File;
use std::io::{BufRead, BufReader, Error};
use regex::Regex;

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn solve_challenge(input_lines : Vec<String>) -> Result<(usize, usize), Error> {
    let re = Regex::new(r"^(\d+)-(\d+) (\w): (\w+)$").unwrap();
    let mut part1_cnt : usize = 0;
    let mut part2_cnt : usize = 0;

    for line in input_lines.iter() {
        let caps = re.captures(line).unwrap();
        let a : usize = caps[1].parse::<usize>().unwrap();
        let b: usize = caps[2].parse::<usize>().unwrap();
        let ch: char = caps[3].parse::<char>().unwrap();
        let password : String = caps[4].to_string();
        let ch_matches : usize = password.matches(ch).count();

        if ch_matches >= a && ch_matches <= b {
            part1_cnt += 1;
        }

        let idx1 = a - 1;
        let idx2 = b - 1;
        let mut part2_cond = false;

        if idx1 <= password.len() - 1 && password.chars().nth(idx1).unwrap() == ch {
            part2_cond = !part2_cond;
        }

        if idx2 <= password.len() - 1 && password.chars().nth(idx2).unwrap() == ch {
            part2_cond = !part2_cond;
        }

        if part2_cond == true {
            part2_cnt += 1;
        }
    }

    return Ok((part1_cnt, part2_cnt));
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 02 ====================================\n");

    let input : Vec<String> = match get_input("input/day02.in") {
        Ok(content) => content,
        Err(err) => return Err(err.into()),
    };

    let (ans1, ans2) = solve_challenge(input).unwrap();

    println!("The answer for part1 is: {}", ans1);
    println!("The answer for part2 is: {}", ans2);

    println!("\n================================================================================\n");

    Ok(())
}
