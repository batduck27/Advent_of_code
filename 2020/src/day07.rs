use std::fs::File;
use std::io::{BufRead, BufReader, Error};
use regex::Regex;
use std::collections::{HashMap, VecDeque, HashSet};

type Graph = HashMap<String, Vec<(String, usize)>>;

fn get_input(filename: &str) -> Result<Vec<String>, Error> {
    BufReader::new(File::open(filename).unwrap()).lines().collect()
}

fn process_input(input: Vec<String>) -> (Graph, Graph) {
    let bag_reg = Regex::new(r"^(?:(\w+ \w+) bags contain)").unwrap();
    let content_reg = Regex::new(r"(?:(\d+) (\w+ \w+) bags?[,\.])").unwrap();
    let mut t_bags_graph = Graph::new();
    let mut bags_graph = Graph::new();

    for line in input {
        let cur_bag = bag_reg.captures(&line).unwrap()[1].to_string();

        for caps in content_reg.captures_iter(&line) {
            t_bags_graph
                .entry(caps[2].to_string())
                .or_insert(Vec::new())
                .push((cur_bag.clone(), caps[1].parse().unwrap()));

            bags_graph
                .entry(cur_bag.clone())
                .or_insert(Vec::new())
                .push((caps[2].to_string(), caps[1].parse().unwrap()));
        }
    }

    (t_bags_graph, bags_graph)
}

fn solve_part1(graph: Graph) -> usize {
    let mut Q: VecDeque<String> = VecDeque::new();
    let mut vis: HashSet<String> = HashSet::new();

    Q.push_back("shiny gold".to_string());

    while !Q.is_empty() {
        let node = Q.pop_front().unwrap();

        if !vis.contains(&node) {
            vis.insert(node.clone());

            for (neigh, _) in graph.get(&node).unwrap_or(&vec![]) {
                if !vis.contains(neigh) {
                    Q.push_back(neigh.to_string());
                }
            }
        }
    }

    vis.len() - 1
}

fn solve_part2(graph: Graph) -> usize {
    let mut Q: VecDeque<(String, usize)> = VecDeque::new();
    let mut ans = 0;

    Q.push_back(("shiny gold".to_string(), 1));

    while !Q.is_empty() {
        let (node, cur_val) = Q.pop_front().unwrap();

        for (neigh, cost) in graph.get(&node).unwrap_or(&vec![]) {
            Q.push_back((neigh.to_string(), cur_val * cost))
        }

        ans += cur_val
    }

    ans - 1
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("\n==================================== Day 07 ====================================\n");

    let input : Vec<String> = match get_input("input/day07.in") {
        Ok(content) => content,
        Err(err) => return Err(err.into()),
    };

    let (t_graph, graph) = process_input(input);

    println!("The answer for part1 is: {}", solve_part1(t_graph));
    println!("The answer for part2 is: {}", solve_part2(graph));

    println!("\n================================================================================\n");

    Ok(())
}
