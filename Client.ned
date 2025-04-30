//
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
