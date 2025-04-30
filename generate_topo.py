#!/usr/bin/env python3
"""
Generate topology file for P2P Network simulation.
This script generates topo.txt file with ring topology and finger table connections.
"""

import math

def generate_topo_file(num_clients):
    """
    Generate the topology file with ring connections and finger table entries
    
    Args:
        num_clients: Number of clients in the network
    """
    with open("topo.txt", "w") as f:
        # Write header
        f.write("# Base ring topology\n")
        
        # Generate ring topology (each node connects to its successor)
        for i in range(num_clients):
            successor = (i + 1) % num_clients
            f.write(f"{i} {successor}\n")
        
        f.write("\n# Finger table connections for O(logN) routing\n")
        
        # Calculate number of bits needed to represent all nodes
        m = math.ceil(math.log2(num_clients))
        
        # Generate finger table connections for each node
        for node in range(num_clients):
            f.write(f"# For node {node}\n")
            
            # Skip the first finger since it's already covered by the ring connections
            for power in range(1, m):
                # Formula: (n + 2^i) mod 2^m
                finger = (node + (1 << power)) % num_clients
                f.write(f"{node} {finger}\n")

    print(f"Generated topo.txt with ring topology and finger tables for {num_clients} clients")

def main():
    # Set the number of clients here (should match Network.ned)
    num_clients = 8
    
    # Generate the topology file
    generate_topo_file(num_clients)

if __name__ == "__main__":
    main()