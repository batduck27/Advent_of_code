use std::fs::File;
use std::io::{BufRead, BufReader, Error};

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 05 ====================================\n");

    let input : Vec<String> = match get_input("input/day05.in") {
        Ok(content) => content,
        Err(err) => return Err(err.into()),
    };

    let mut seat_numbers: Vec<usize> = input
        .iter()
        .map(|line| {
                let seat_radix_str = line
                    .chars()
                    .map(|x|
                            match x {
                                'F' => '0',
                                'B' => '1',
                                'R' => '1',
                                'L' => '0',
                                _ => panic!("Shouldn't happen {:?}", x)
                            }).collect::<String>();
                usize::from_str_radix(seat_radix_str.as_str(), 2).unwrap()
        })
        .collect();

    seat_numbers.sort();

    let mut ans: usize = 0;
    for i in 1..seat_numbers.len() {
        if seat_numbers[i - 1] + 1 != seat_numbers[i] {
            ans = seat_numbers[i] - 1;
            break;
        }
    }

    println!("The answer for part1 is: {:?}", seat_numbers.last().unwrap());
    println!("The answer for part2 is: {:?}", ans);

    println!("\n================================================================================\n");

    Ok(())
}
