//
// B22CS086 B22CS018
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __B22CS086_B22CS018_ASG_3_CLIENT_H_
#define __B22CS086_B22CS018_ASG_3_CLIENT_H_

#include <omnetpp.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include "ClientMsg_m.h"

using namespace omnetpp;

/**
 * Client module for P2P distributed computing network
 */
class Client : public cSimpleModule
{
private:
    // Client identification
    int clientId;
    std::string configFile;
    std::string outputFile;
    std::ofstream logFile;

    // Network parameters
    int numClients;
    int numSubtasks;
    int numTasks;
    std::vector<int> dataArray;
    std::vector<int> fingerTable;
    std::set<int> peerConnections; // Directly connected peers

    // Task management
    std::map<int, int> subtaskResults;
    int completedTasks;
    bool taskInProgress;

    // Gossip protocol
    std::unordered_map<std::string, bool> processedGossips;
    std::set<int> receivedGossips;

    // Performance metrics
    int messageHops;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // Configuration and setup
    bool loadConfig();
    void setupConnections();
    void createFingerTable();

    // Task processing
    void initiateTask();
    void computeMaximum(ClientMsg *msg);
    void processResult(ClientMsg *msg);
    std::vector<std::vector<int>> divideArray(const std::vector<int> &array, int parts);

    // Message handling
    void forwardMessage(ClientMsg *msg);
    int findNextHop(int targetId);

    // Gossip protocol
    void sendGossip();
    void processGossip(ClientMsg *msg);
    void forwardGossip(ClientMsg *msg);
    void exchangeGossips();  // New method for periodic gossip exchange

    // Monitoring and status
    void checkStatus();

    // Utilities
    void logMessage(const std::string &message);
};

#endif
