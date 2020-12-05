use std::fs;
use std::collections::HashMap;
use itertools::Itertools;
use regex::Regex;

#[macro_use] extern crate lazy_static;

macro_rules! vec_of_strings {
    ($($x:expr),*) => (vec![$($x.to_string()),*]);
}

macro_rules! vec_of_regexes {
    ($($x:expr),*) => (vec![$(Regex::new($x).unwrap()),*]);
}

fn get_input(filename: &str) -> Vec<String> {
    let input_as_string = fs::read_to_string(filename).unwrap();

    input_as_string
        .split("\n\n")
        .map(|chunk|
            str::replace(chunk, "\n", " ").trim().to_string())
        .collect()
}

fn get_passports() -> Vec<HashMap<String, String>> {
    get_input("input/day04.in")
        .iter()
        .map(|chunk|
                chunk.split_whitespace().map(|field|
                    field.split(":").map(str::to_string).collect_tuple().unwrap()
            )
            .collect::<HashMap<String, String>>()
        )
        .collect()
}

fn part1_check(passport_data: &HashMap<String, String>) -> bool {
    let rq_fields : Vec<String>= vec_of_strings!["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];

    rq_fields.iter().fold(true, |valid, field| valid & passport_data.contains_key(field))
}

fn part2_check(passport_data: &HashMap<String, String>) -> bool {
    lazy_static! {
        static ref FIELDS: Vec<String> = vec_of_strings!["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];
        static ref FIELD_CONTENT_REGS: Vec<Regex> = vec_of_regexes!["^19[2-9][0-9]|20(:?[0-1][0-9]|20)$",
            "^20(:?1[0-9]|20)$", "^20(:?2[0-9]|30)$", "^(:?(:?59|6[0-9]|7[0-6])in)|(:?1(:?[5-8][0-9]|9[0-3])cm)$",
            "^#[0-9a-f]{6}$", "^amb|blu|brn|gry|grn|hzl|oth$", "^[0-9]{9}$"];

        static ref FIELD_REGS: HashMap<&'static String, &'static Regex> = FIELDS
            .iter()
            .zip(FIELD_CONTENT_REGS.iter())
            .collect::<Vec<(&String, &Regex)>>()
            .iter()
            .cloned()
            .collect();
    };

    for (&field, &field_re) in FIELD_REGS.iter() {
        if !passport_data.contains_key(field) {
            return false;
        }

        if !field_re.is_match(passport_data.get(field).unwrap()) {
            return false;
        }
    }

    return true;
}

fn count_valid_passports(passports: &Vec<HashMap<String, String>>, check: &dyn Fn(&HashMap<String, String>) -> bool) -> usize {
    passports
        .into_iter()
        .filter(|pass_data| check(pass_data))
        .count()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 04 ====================================\n");

    let passports = get_passports();

    println!("The answer for part1 is: {}", count_valid_passports(&passports, &part1_check));
    println!("The answer for part2 is: {}", count_valid_passports(&passports, &part2_check));

    println!("\n================================================================================\n");

    Ok(())
}
