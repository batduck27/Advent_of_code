use std::fs::File;
use std::io::{BufRead, BufReader};
use modinverse::modinverse;

fn get_input(filename: &str) -> (u64, Vec<Option<u64>>) {
    let input: Vec<String> = BufReader::new(File::open(filename).unwrap())
        .lines()
        .map(|line| line.unwrap())
        .collect();

    let mut iter = input.iter();
    let timestamp: u64 = iter.next().unwrap().parse().unwrap();
    let buses: Vec<Option<u64>> = iter
        .next()
        .unwrap()
        .split(",")
        .fold(Vec::new(), |mut acc, token| {
            if let Ok(num) = token.parse() {
                acc.push(Some(num));
            } else {
                acc.push(None);
            }

            acc
        });

    (timestamp, buses)
}

fn solve_part1(timestamp: &u64, buses: &Vec<Option<u64>>) -> u64 {
    let mut min_diff: u64 = u64::MAX;
    let mut ans: u64 = u64::MAX;

    for bus_option in buses {
        if let None = bus_option {
            continue;
        }

        let bus = bus_option.unwrap();
        let remainder: u64 = (*timestamp as f64 / bus as f64).ceil() as u64;
        let diff = remainder * bus - timestamp;

        if diff < min_diff {
            min_diff = diff;
            ans = diff * bus
        }
    }

    ans
}

fn solve_part2(buses: &Vec<Option<u64>>) -> u64 {
    let mut num: Vec<i128> = vec![];
    let mut rem: Vec<i128> = vec![];
    let mut prod: i128 = 1;
    let mut ans = 0;

    for i in 0..buses.len() {
        if let Some(bus) = buses[i] {
            num.push(bus as i128);

            let mut curr_rem = -(i as i128);
            while curr_rem < 0 {
                curr_rem += num.last().unwrap();
            }

            rem.push(curr_rem);
            prod *= bus as i128;
        }
    }

    for i in 0..num.len() {
        let pp: i128 = prod / num[i];
        ans = (ans + ((rem[i] * modinverse(pp, num[i]).unwrap() * pp) % prod)) % prod;
    }

    (ans % prod) as u64
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 13 ====================================\n");

    let (timestamp, buses) = get_input("input/day13.in");

    println!("The answer for part1 is: {}", solve_part1(&timestamp, &buses));
    println!("The answer for part2 is: {}", solve_part2(&buses));

    println!("\n================================================================================\n");

    Ok(())
}
