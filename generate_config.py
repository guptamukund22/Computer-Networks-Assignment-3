#!/usr/bin/env python3
"""
Generate configuration file for P2P Network simulation.
This script generates config.txt file with simulation parameters and data array.
"""

import random

def generate_config_file(num_clients, num_subtasks, total_tasks, array_size, max_value=100):
    """
    Generate the configuration file with simulation parameters and data array
    
    Args:
        num_clients: Number of clients in the network
        num_subtasks: Number of subtasks to divide the work into
        total_tasks: Total number of tasks to be processed
        array_size: Size of the data array to be processed
        max_value: Maximum value for array elements
    """
    with open("config.txt", "w") as f:
        # Write number of clients
        f.write(f"{num_clients}\n")
        
        # Write number of subtasks
        f.write(f"{num_subtasks}\n")
        
        # Write total number of tasks
        f.write(f"{total_tasks}\n")
        
        # Generate random data array
        data_array = [random.randint(1, max_value) for _ in range(array_size)]
        
        # Write data array
        f.write(" ".join(map(str, data_array)))
        
    print(f"Generated config.txt with {num_clients} clients, {num_subtasks} subtasks, " +
          f"{total_tasks} tasks, and data array of size {array_size}")

def main():
    # Simulation parameters
    num_clients = 8
    num_subtasks = 16
    total_tasks = 1
    array_size = 30
    max_value = 100
    
    # Generate the configuration file
    generate_config_file(num_clients, num_subtasks, total_tasks, array_size, max_value)

if __name__ == "__main__":
    main()