# OMNeT++ P2P Network Simulation - Assignment 3

Banoth Sri Kowshika Raj : B22CS018
Mukund Gupta : B22CS086

This project simulates a peer-to-peer network where each client executes a distributed task (finding the maximum element in an array) by dividing it into subtasks and distributing them across the network. The implementation includes optimized O(log N) message routing based on Chord DHT principles and a gossip protocol for message dissemination.

## Files Included

- `Client.ned`: Network module definition for client nodes
- `Network.ned`: Network topology definition
- `ClientMsg.msg`: Message definition for client communication
- `Client.cc`: Implementation of client functionality
- `Makefile`: Build configuration
- `omnetpp.ini`: Simulation configuration
- `config.txt`: Task configuration (number of clients, subtasks, array data)
- `topo.txt`: Network topology definition

## Compilation Steps

1. Make sure OMNeT++ is installed and configured properly
2. Navigate to the project directory
3. Run the following commands:

```bash
$ opp_msgc ClientMsg.msg
$ make
```

## Running the Simulation

```bash
$ ./assignment3 -f omnetpp.ini
```

## Configuration

### config.txt Format
The configuration file `config.txt` contains:
- Line 1: Number of clients (N)
- Line 2: Number of subtasks (x)
- Line 3: Number of tasks to execute
- Line 4: Array data (space-separated integers)

### topo.txt Format
The topology file `topo.txt` defines the network connections:
- Each line represents a connection with format: `sourceNodeId destinationNodeId`
- The first section creates the base ring topology
- The additional connections optimize routing to achieve O(log N) complexity

## Implementation Details

1. **Network Topology**: The network is organized as a ring where each client connects to its successor and predecessor. Additional connections are added based on Chord DHT finger tables to achieve O(log N) routing.

2. **Task Distribution**: A task (finding the max element in an array) is divided into x subtasks. Each subtask with ID i is assigned to client ID i%N.

3. **Message Routing**: Messages are routed using the finger table entries to achieve O(log N) complexity instead of O(N) in the basic ring topology.

4. **Gossip Protocol**: After task completion, clients propagate gossip messages throughout the network. Each client forwards messages only once to avoid loops.

5. **Output**: Results and messages are logged to both console and outputfile.txt.
