use std::fs::File;
use std::io::{BufRead, BufReader, Error};
use itertools::Itertools;
use std::collections::HashSet;

struct RegisterSet {
    acc: i64,
    pc: u64
}

trait OpCode {
    fn exec(&self, regs: &mut RegisterSet);
}

struct JMP {
    offset: i64
}

struct ACC {
    value: i64
}

struct NOP {
    value: i64
}

struct GameConsole {
    regs: RegisterSet,
    instructions: Vec<Box<dyn OpCode>>,
    executed: HashSet<u64>
}

enum HaltReason {
    Dirty(i64),
    Finish(i64)
}

impl GameConsole {
    fn reset_console(&mut self) {
        self.regs.acc = 0;
        self.regs.pc = 0;

        self.executed = HashSet::new();
    }

    fn exec_instructions(&mut self) -> HaltReason {
        self.reset_console();

        loop {
            if self.executed.contains(&self.regs.pc) {
                return HaltReason::Dirty(self.regs.acc);
            }

            if self.regs.pc as usize == self.instructions.len() {
                return HaltReason::Finish(self.regs.acc);
            }

            self.executed.insert(self.regs.pc);
            self.instructions[self.regs.pc as usize].exec(&mut self.regs)
        }
    }
}

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn get_instruction(tokens: (&str, &str)) -> Box<dyn OpCode> {
    match tokens.0 {
        "acc" => Box::new(ACC{value: tokens.1.parse().unwrap()}),
        "jmp" => Box::new(JMP{offset: tokens.1.parse().unwrap()}),
        "nop" => Box::new(NOP{value: tokens.1.parse().unwrap()}),
        _ => panic!("Invalid instruction: {:?}", tokens.0),
    }
}

fn solve_part1(console: &mut GameConsole) -> i64 {
    match console.exec_instructions() {
        HaltReason::Dirty(acc) => acc,
        _ => panic!("Invalid result."),
    }
}

fn solve_part2(console: &mut GameConsole) -> i64 {
    unimplemented!();
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 08 ====================================\n");

    let input: Vec<String> = match get_input("input/day08.in") {
        Ok(content) => content,
        Err(err) => return Err(err.into()),
    };
    let mut console = GameConsole{
        regs: RegisterSet{acc: 0, pc: 0},
        instructions: input
            .iter()
            .map(|line| get_instruction(line.split_whitespace().collect_tuple().unwrap()))
            .collect(),
        executed: HashSet::new()
    };

    println!("The answer for part1 is: {}", solve_part1(&mut console));
    println!("The answer for part2 is: {}", solve_part2(&mut console));

    println!("\n================================================================================\n");

    Ok(())
}

impl OpCode for ACC {
    fn exec(&self, regs: &mut RegisterSet) {
        regs.acc += self.value;
        regs.pc += 1;
    }
}

impl OpCode for JMP {
    fn exec(&self, regs: &mut RegisterSet) {
        regs.pc = regs.pc.wrapping_add(self.offset as u64);
    }
}

impl OpCode for NOP {
    fn exec(&self, regs: &mut RegisterSet) {
        regs.pc += 1;
    }
}