//
// Generated Network.ned file
//

network P2PNetwork {
    parameters:
        int numClients = default(8);
    
    submodules:
        client[numClients]: Client {
            clientId = index;
            configFile = "config.txt";
        }

    connections allowunconnected:
}
