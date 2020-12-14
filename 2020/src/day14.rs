use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, BufReader, Error};
use regex::Regex;

enum Instruction {
    SetMask(String),
    MemoryWrite(usize, u64)
}

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn get_instructions() -> Vec<Instruction> {
    let input: Vec<String> = match get_input("input/day14.in") {
        Ok(content) => content,
        Err(err) => panic!("Couldn't parse input file: {:?}", err),
    };

    let mask_reg = Regex::new(r"^mask\s=\s([10X]{36})$").unwrap();
    let mem_reg = Regex::new(r"^mem\[(\d+)\]\s=\s(\d+)$").unwrap();

    input
        .iter()
        .map(|line| {
            if mask_reg.is_match(line) {
                Instruction::SetMask(mask_reg.captures(line).unwrap()[1].to_string())
            } else if mem_reg.is_match(line) {
                let caps = mem_reg.captures(line).unwrap();
                Instruction::MemoryWrite(caps[1].parse().unwrap(), caps[2].parse().unwrap())
            } else {
                panic!("Invalid instruction from line: {:?}", line)
            }
        })
        .collect()
}

fn set_bit(val: &mut u64, bit: usize) {
    *val |= (1 << bit);
}

fn unset_bit(val: &mut u64, bit: usize) {
    *val &= !(1 << bit);
}

fn solve_part1(program: &Vec<Instruction>) -> u64 {
    let mut mask: &str = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    let mut memory: HashMap<usize, u64> = HashMap::new();

    for ins in program {
        match ins {
            Instruction::SetMask(new_mask) => mask = new_mask,
            Instruction::MemoryWrite(idx, val) => {
                let mut masked_val = *val;

                for (i, ch) in mask.chars().enumerate() {
                    match ch {
                        '1' => set_bit(&mut masked_val, mask.len() - i - 1),
                        '0' => unset_bit(&mut masked_val, mask.len() - i - 1),
                        _ => (),
                    }
                }

                memory.insert(*idx, masked_val);
            }
        }
    }

    memory.values().sum::<u64>()
}

fn add_to_mem_part2(memory: &mut HashMap<usize, u64>, addr: &u64, val: &u64, mask: &Vec<char>, start_bit_pos: &usize) {
    let mut bit_pos = *start_bit_pos;

    while bit_pos < mask.len() && mask[bit_pos] != 'X' {
        bit_pos += 1;
    }

    if bit_pos < mask.len() {
        let mut new_addr = *addr;

        set_bit(&mut new_addr, mask.len() - bit_pos - 1);
        add_to_mem_part2(memory, &new_addr, val, mask, &(bit_pos + 1));

        unset_bit(&mut new_addr, mask.len() - bit_pos - 1);
        add_to_mem_part2(memory, &new_addr, val, mask, &(bit_pos + 1));
    } else {
        memory.insert(*addr as usize, *val);
    }
}

fn solve_part2(program: &Vec<Instruction>) -> u64 {
    let mut mask: Vec<char> = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX".chars().collect();
    let mut memory: HashMap<usize, u64> = HashMap::new();

    for ins in program {
        match ins {
            Instruction::SetMask(new_mask) => mask = new_mask.chars().collect(),
            Instruction::MemoryWrite(idx, val) => {
                let mut masked_addr = *idx as u64;

                for i in 0..mask.len() {
                    match mask[i] {
                        '1' => set_bit(&mut masked_addr, mask.len() - i - 1),
                        _ => (),
                    }
                }

                add_to_mem_part2(&mut memory, &masked_addr, val, &mask, &0);
            }
        }
    }

    memory.values().sum::<u64>()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 14 ====================================\n");

    let program = get_instructions();

    println!("The answer for part1 is: {}", solve_part1(&program));
    println!("The answer for part2 is: {}", solve_part2(&program));

    println!("\n================================================================================\n");

    Ok(())
}
