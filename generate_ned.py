#!/usr/bin/env python3
"""
Generate NED files for P2P Network simulation.
This script generates Network.ned and Client.ned files in the current directory.
"""

def generate_client_ned():
    """Generate the Client.ned file in the current directory"""
    content = """//
// Generated Client.ned file
//

simple Client {
    parameters:
        int clientId;
        string configFile = default("config.txt");
        string outputFile = default("outputfile.txt");
    
    gates:
        inout gate[];
}
"""
    
    # Write to file
    with open("Client.ned", "w") as f:
        f.write(content)
    
    print("Generated Client.ned in current directory")

def generate_network_ned(num_clients):
    """Generate the Network.ned file in the current directory"""
    content = f"""//
// Generated Network.ned file
//

network P2PNetwork {{
    parameters:
        int numClients = default({num_clients});
    
    submodules:
        client[numClients]: Client {{
            clientId = index;
            configFile = "config.txt";
        }}

    connections allowunconnected:
}}
"""
    
    # Write to file
    with open("Network.ned", "w") as f:
        f.write(content)
    
    print(f"Generated Network.ned with {num_clients} clients in current directory")

def main():
    # Set the number of clients here
    num_clients = 8
    
    # Generate the NED files
    generate_client_ned()
    generate_network_ned(num_clients)

if __name__ == "__main__":
    main()