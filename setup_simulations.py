#!/usr/bin/env python3
"""
Setup script for P2P Network simulation.
This script runs all the generation scripts to set up the simulation environment.
"""

import os
import sys
import importlib.util

def import_module_from_file(module_name, file_path):
    """Import a module from file path"""
    spec = importlib.util.spec_from_file_location(module_name, file_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = module
    spec.loader.exec_module(module)
    return module

def main():
    # Define simulation parameters
    num_clients = 8
    num_subtasks = 16
    total_tasks = 1
    array_size = 30
    max_value = 100
    
    # Current directory
    current_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Import and run generate_ned.py
    print("Generating NED files...")
    generate_ned = import_module_from_file("generate_ned", os.path.join(current_dir, "generate_ned.py"))
    generate_ned.main()
    
    # Import and run generate_topo.py
    print("\nGenerating topology file...")
    generate_topo = import_module_from_file("generate_topo", os.path.join(current_dir, "generate_topo.py"))
    # Make sure to use the same number of clients
    generate_topo.generate_topo_file(num_clients)
    
    # Import and run generate_config.py
    print("\nGenerating configuration file...")
    generate_config = import_module_from_file("generate_config", os.path.join(current_dir, "generate_config.py"))
    # Use all the parameters we defined
    generate_config.generate_config_file(num_clients, num_subtasks, total_tasks, array_size, max_value)
    
    print("\nSimulation setup complete. You can now run the OMNeT++ simulation.")

if __name__ == "__main__":
    main()