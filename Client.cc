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

#include "Client.h"

Define_Module(Client);

void Client::initialize() {
    // Client identification
    clientId = par("clientId");
    configFile = par("configFile").stringValue();
    outputFile = par("outputFile").stringValue();

    // Open output file with unique name
    std::string logFileName = outputFile + "_" + std::to_string(clientId) + ".txt";
    logFile.open(logFileName.c_str(), std::ios::out);
    if (!logFile.is_open()) {
        EV << "Failed to open output file: " << logFileName << "\n";
    }

    // Performance metrics
    messageHops = 0;

    logMessage("Client " + std::to_string(clientId) + " initialized");

    // Load configuration
    if (!loadConfig()) {
        error("Failed to load configuration");
        return;
    }

    // Setup connections and routing
    setupConnections();
    createFingerTable();

    // Initialize state variables
    taskInProgress = false;
    completedTasks = 0;

    // Always add self to received gossips
    receivedGossips.insert(clientId);

    // Schedule first task (only for client 0)
    if (clientId == 0) {
        scheduleAt(simTime() + 1.0, new cMessage("startTask"));
    }

    // Schedule periodic gossip exchange
    scheduleAt(simTime() + 3.0, new cMessage("exchangeGossip"));

    // Schedule periodic status check
    scheduleAt(simTime() + 5.0, new cMessage("statusCheck"));
}

bool Client::loadConfig() {
    std::ifstream config(configFile.c_str());
    if (!config.is_open()) {
        return false;
    }

    std::string line;

    // Read number of clients
    if (!std::getline(config, line)) return false;
    numClients = std::stoi(line);

    // Read number of subtasks
    if (!std::getline(config, line)) return false;
    numSubtasks = std::stoi(line);

    // Read total tasks
    if (!std::getline(config, line)) return false;
    numTasks = std::stoi(line);

    // Read data array
    if (!std::getline(config, line)) return false;
    std::stringstream ss(line);
    int value;
    while (ss >> value) {
        dataArray.push_back(value);
    }

    config.close();

    logMessage("Configuration loaded: " + std::to_string(numClients) + " clients, " +
              std::to_string(numSubtasks) + " subtasks, " +
              std::to_string(dataArray.size()) + " elements in data array");
    return true;
}

void Client::setupConnections() {
    std::ifstream topoFile("topo.txt");
    if (!topoFile.is_open()) {
        error("Cannot open topology file");
        return;
    }

    std::string line;
    while (std::getline(topoFile, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        int from, to;
        ss >> from >> to;

        // Only create connections where this client is the source
        if (from == clientId) {
            // Find target module
            cModule *network = getParentModule();
            cModule *targetModule = network->getSubmodule("client", to);
            if (!targetModule) continue;

            // Setup gates for connection
            int gateIndex = gateSize("gate");
            setGateSize("gate", gateIndex + 1);
            cGate *outGate = gate("gate$o", gateIndex);
            cGate *inGate = gate("gate$i", gateIndex);

            // Setup target gates
            int targetGateIndex = targetModule->gateSize("gate");
            targetModule->setGateSize("gate", targetGateIndex + 1);

            // Connect gates
            outGate->connectTo(targetModule->gate("gate$i", targetGateIndex));
            targetModule->gate("gate$o", targetGateIndex)->connectTo(inGate);

            // Record connection
            peerConnections.insert(to);
            logMessage("Connected to client " + std::to_string(to));
        }
    }
    topoFile.close();
}

void Client::createFingerTable() {
    int bits = ceil(log2(numClients));
    fingerTable.resize(bits);

    for (int i = 0; i < bits; i++) {
        fingerTable[i] = (clientId + (1 << i)) % numClients;
    }

    logMessage("Finger table initialized with " + std::to_string(bits) + " entries");
}

void Client::handleMessage(cMessage *msg) {
    // Handle self-messages (timers)
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "startTask") == 0) {
            initiateTask();
            delete msg;
        }
        else if (strcmp(msg->getName(), "statusCheck") == 0) {
            checkStatus();
            // Reschedule next check
            scheduleAt(simTime() + 5.0, new cMessage("statusCheck"));
            delete msg;
        }
        else if (strcmp(msg->getName(), "exchangeGossip") == 0) {
            // Periodic gossip exchange to improve propagation
            exchangeGossips();
            // Reschedule next exchange
            scheduleAt(simTime() + 3.0, new cMessage("exchangeGossip"));
            delete msg;
        }
        else {
            // This is either a subtask or a client message that was scheduled locally
            ClientMsg *clientMsg = dynamic_cast<ClientMsg *>(msg);
            if (clientMsg) {
                // Handle based on the message type
                if (clientMsg->getDestId() == clientId) {
                    // Process message meant for this client
                    if (clientMsg->isGossip()) {
                        processGossip(clientMsg);
                    }
                    else if (clientMsg->isResult()) {
                        processResult(clientMsg);
                    }
                    else {
                        // Process computation request
                        computeMaximum(clientMsg);
                    }
                }
                else {
                    // Forward to destination if not for this client
                    forwardMessage(clientMsg);
                }
                delete msg;
            }
            else {
                // Unknown self message type
                delete msg;
            }
        }
        return;
    }

    // Handle messages from other clients
    ClientMsg *clientMsg = dynamic_cast<ClientMsg *>(msg);
    if (clientMsg) {
        // Update hop count statistics
        messageHops += clientMsg->getHopCount();

        // Handle message based on target
        if (clientMsg->getDestId() == clientId) {
            // Message is for this client
            if (clientMsg->isGossip()) {
                processGossip(clientMsg);
            }
            else if (clientMsg->isResult()) {
                processResult(clientMsg);
            }
            else {
                // Process computation request
                computeMaximum(clientMsg);
            }
        }
        else {
            // Forward message to next hop
            forwardMessage(clientMsg);
        }
    }

    delete msg;
}

void Client::initiateTask() {
    if (taskInProgress) return;

    taskInProgress = true;
    logMessage("Starting task " + std::to_string(completedTasks + 1));

    // Divide array into subtasks
    std::vector<std::vector<int>> subtasks = divideArray(dataArray, numSubtasks);

    // Distribute subtasks
    for (int i = 0; i < numSubtasks; i++) {
        int targetNode = (clientId + i) % numClients;

        // Create subtask message
        ClientMsg *msg = new ClientMsg("subtask");
        msg->setSourceId(clientId);
        msg->setDestId(targetNode);
        msg->setTaskId(completedTasks);
        msg->setSubtaskId(i);
        msg->setHopCount(0);

        // Set array data
        msg->setArrayArraySize(subtasks[i].size());
        for (size_t j = 0; j < subtasks[i].size(); j++) {
            msg->setArray(j, subtasks[i][j]);
        }

        logMessage("Sending subtask " + std::to_string(i) +
                  " to client " + std::to_string(targetNode) +
                  " with " + std::to_string(subtasks[i].size()) + " elements");

        // Handle local vs remote routing
        if (targetNode == clientId) {
            // Local computation (as a self-message)
            scheduleAt(simTime() + 0.01 * i, msg);
        } else {
            // This is a message for another client - send it directly through gates
            bool sent = false;

            // Try each outgoing gate
            for (int g = 0; g < gateSize("gate$o"); g++) {
                cGate *gate = this->gate("gate$o", g);
                if (!gate->isConnected()) continue;

                cModule *target = gate->getPathEndGate()->getOwnerModule();
                if (!target) continue;

                int connectedId = target->par("clientId");

                // Check if we have a direct connection to the target
                if (connectedId == targetNode) {
                    send(msg, gate);
                    sent = true;
                    break;
                }
            }

            // If no direct connection, use routing
            if (!sent) {
                // Schedule for processing by the router
                scheduleAt(simTime() + 0.01 * i, msg);
            }
        }
    }
}

std::vector<std::vector<int>> Client::divideArray(const std::vector<int>& array, int parts) {
    std::vector<std::vector<int>> result(parts);
    int size = array.size();

    if (size == 0) return result;

    // Calculate base size and remainder
    int baseSize = size / parts;
    int remainder = size % parts;

    // Divide elements among parts
    int start = 0;
    for (int i = 0; i < parts; i++) {
        int partSize = baseSize + (i < remainder ? 1 : 0);

        // Ensure we don't go beyond array bounds
        if (start >= size) {
            result[i].clear();
            continue;
        }

        // Calculate end position
        int end = std::min(start + partSize, size);

        // Copy elements
        result[i].assign(array.begin() + start, array.begin() + end);
        start = end;
    }

    return result;
}

void Client::computeMaximum(ClientMsg *msg) {
    // Extract array data
    std::vector<int> array;
    for (int i = 0; i < msg->getArrayArraySize(); i++) {
        array.push_back(msg->getArray(i));
    }

    // Compute maximum
    int maxVal = INT_MIN;
    if (!array.empty()) {
        maxVal = *std::max_element(array.begin(), array.end());
    }

    // Create result message
    ClientMsg *resultMsg = new ClientMsg("result");
    resultMsg->setSourceId(clientId);
    resultMsg->setDestId(msg->getSourceId());
    resultMsg->setTaskId(msg->getTaskId());
    resultMsg->setSubtaskId(msg->getSubtaskId());
    resultMsg->setResult(maxVal);
    resultMsg->setIsResult(true);
    resultMsg->setHopCount(0);

    logMessage("Computed max for subtask " + std::to_string(msg->getSubtaskId()) +
              " of task " + std::to_string(msg->getTaskId()) +
              ": " + std::to_string(maxVal));

    // Send result back to source (through direct gates if possible)
    int sourceId = msg->getSourceId();
    bool sent = false;

    // Try direct connection first
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        if (peerId == sourceId) {
            // Direct connection to source exists
            send(resultMsg, gate);
            sent = true;
            break;
        }
    }

    // If no direct connection, use routing
    if (!sent) {
        // Schedule the result message for routing
        scheduleAt(simTime() + 0.01, resultMsg);
    }
}

void Client::processResult(ClientMsg *msg) {
    int taskId = msg->getTaskId();
    int subtaskId = msg->getSubtaskId();
    int result = msg->getResult();

    logMessage("Received result for subtask " + std::to_string(subtaskId) +
              " of task " + std::to_string(taskId) +
              ": " + std::to_string(result));

    // Store result
    subtaskResults[subtaskId] = result;

    // Check if all subtasks are complete
    if (subtaskResults.size() == numSubtasks) {
        // Find maximum value across all subtasks
        int finalMax = INT_MIN;
        for (const auto& pair : subtaskResults) {
            finalMax = std::max(finalMax, pair.second);
        }

        logMessage("Task " + std::to_string(completedTasks) +
                  " completed. Final max: " + std::to_string(finalMax));

        // Clear results for next task
        subtaskResults.clear();
        completedTasks++;

        // Broadcast task completion via gossip
        sendGossip();

        // Prepare for next task if any
        if (completedTasks < numTasks) {
            taskInProgress = false;
            scheduleAt(simTime() + 1.0, new cMessage("startTask"));
        }
    }
}

void Client::sendGossip() {
    // Create timestamp-based gossip message with clearly formatted content
    std::ostringstream oss;
    oss << simTime().dbl() << ":" << clientId << ":task" << completedTasks;
    std::string gossipContent = oss.str();

    logMessage("Broadcasting gossip: " + gossipContent);

    // Create gossip message
    ClientMsg *gossipMsg = new ClientMsg("gossip");
    gossipMsg->setSourceId(clientId);
    gossipMsg->setIsGossip(true);
    gossipMsg->setGossipContent(gossipContent.c_str());
    gossipMsg->setHopCount(0);

    // Record message to avoid duplicates
    processedGossips[gossipContent] = true;

    // Make sure we count ourselves
    if (receivedGossips.find(clientId) == receivedGossips.end()) {
        receivedGossips.insert(clientId);
    }

    // Send to all direct connections with small random delays
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        // Get the connected module through this gate
        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        // Create copy for each peer
        ClientMsg *copy = gossipMsg->dup();
        copy->setDestId(peerId);

        // Add random delay to avoid synchronization issues
        double delay = uniform(0.01, 0.05);
        sendDelayed(copy, delay, gate);

        logMessage("Sent gossip to client " + std::to_string(peerId));
    }

    delete gossipMsg;
}

void Client::processGossip(ClientMsg *msg) {
    std::string content = msg->getGossipContent();

    // Skip if already processed
    if (processedGossips.find(content) != processedGossips.end()) {
        return;
    }

    // Mark as processed
    processedGossips[content] = true;

    // Parse content to extract source client ID
    std::istringstream iss(content);
    std::string timestamp, sourceIdStr, taskInfo;
    std::getline(iss, timestamp, ':');
    std::getline(iss, sourceIdStr, ':');
    std::getline(iss, taskInfo, ':');

    int sourceId = std::stoi(sourceIdStr);

    // Add to received gossips if new
    if (receivedGossips.find(sourceId) == receivedGossips.end()) {
        receivedGossips.insert(sourceId);
        logMessage("Received new gossip from client " + std::to_string(sourceId) +
                  " (total: " + std::to_string(receivedGossips.size()) +
                  "/" + std::to_string(numClients) + ")");
    }

    // Extract task info to update local task status
    if (taskInfo.substr(0, 4) == "task") {
        int taskNumber = std::stoi(taskInfo.substr(4));
        if (sourceId != clientId && taskNumber > completedTasks) {
            // Update our local task completion count for statistics
            logMessage("Updated task completion status from gossip");
            completedTasks = taskNumber;
        }
    }

    // Forward to neighbors
    forwardGossip(msg);
}

void Client::forwardGossip(ClientMsg *msg) {
    // Don't forward if hop count too high
    if (msg->getHopCount() >= 2 * numClients) {  // Increased to ensure full propagation
        return;
    }

    // Get original source of the gossip
    std::string content = msg->getGossipContent();
    std::istringstream iss(content);
    std::string timestamp, sourceIdStr;
    std::getline(iss, timestamp, ':');
    std::getline(iss, sourceIdStr, ':');

    // Forward to all peers except where we got it from
    int immediateSourceId = msg->getSourceId();

    // Loop through all output gates
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        // Don't send back to immediate source
        if (peerId != immediateSourceId) {
            // Create a copy with updated fields
            ClientMsg *copy = msg->dup();
            copy->setSourceId(clientId); // We are now the source
            copy->setDestId(peerId);
            copy->setHopCount(msg->getHopCount() + 1);

            // Add a small delay to avoid synchronization issues
            double delay = uniform(0.01, 0.05);
            sendDelayed(copy, delay, gate);

            logMessage("Forwarded gossip to client " + std::to_string(peerId));
        }
    }
}

void Client::forwardMessage(ClientMsg *msg) {
    int destId = msg->getDestId();

    // If message is for this client, process locally
    if (destId == clientId) {
        // Create a copy and schedule for local processing
        ClientMsg *copy = msg->dup();
        scheduleAt(simTime() + 0.01, copy);
        return;
    }

    // Try direct connection first
    bool sent = false;
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        if (peerId == destId) {
            // Direct connection exists
            msg->setHopCount(msg->getHopCount() + 1);
            send(msg->dup(), gate);
            sent = true;
            break;
        }
    }

    if (sent) return;

    // Use finger table for routing if no direct connection
    int nextHop = findNextHop(destId);

    // Find the gate to the next hop
    sent = false;
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        if (peerId == nextHop) {
            msg->setHopCount(msg->getHopCount() + 1);
            send(msg->dup(), gate);
            sent = true;
            break;
        }
    }

    if (sent) return;

    // If no routing path found yet, try successor
    int successor = (clientId + 1) % numClients;
    for (int i = 0; i < gateSize("gate$o"); i++) {
        cGate *gate = this->gate("gate$o", i);
        if (!gate->isConnected()) continue;

        cModule *target = gate->getPathEndGate()->getOwnerModule();
        if (!target) continue;

        int peerId = target->par("clientId");

        if (peerId == successor) {
            msg->setHopCount(msg->getHopCount() + 1);
            send(msg->dup(), gate);
            sent = true;
            break;
        }
    }

    if (!sent) {
        // Could not route message
        logMessage("ERROR: Cannot route message to " + std::to_string(destId));
    }
}

int Client::findNextHop(int targetId) {
    // Check if target is between this node and successor
    int successor = (clientId + 1) % numClients;
    if ((targetId > clientId && targetId <= successor) ||
        (successor < clientId && (targetId > clientId || targetId <= successor))) {
        return successor;
    }

    // Search finger table for closest preceding node
    for (int i = fingerTable.size() - 1; i >= 0; i--) {
        int finger = fingerTable[i];
        if ((finger > clientId && finger < targetId) ||
            (targetId < clientId && (finger > clientId || finger < targetId))) {
            return finger;
        }
    }

    // Default to successor
    return successor;
}

void Client::exchangeGossips() {
    // If we haven't received gossips from all nodes, actively propagate what we know
    if (receivedGossips.size() < numClients) {
        logMessage("Performing periodic gossip exchange, currently know " +
                  std::to_string(receivedGossips.size()) + "/" +
                  std::to_string(numClients) + " clients");

        // Create a special gossip message that contains our knowledge
        std::ostringstream oss;
        oss << simTime().dbl() << ":" << clientId << ":update";
        std::string gossipContent = oss.str();

        // Create gossip message
        ClientMsg *gossipMsg = new ClientMsg("gossipUpdate");
        gossipMsg->setSourceId(clientId);
        gossipMsg->setIsGossip(true);
        gossipMsg->setGossipContent(gossipContent.c_str());
        gossipMsg->setHopCount(0);

        // Send to random subset of neighbors to avoid flooding
        std::vector<int> connectedPeers;
        for (int i = 0; i < gateSize("gate$o"); i++) {
            cGate *gate = this->gate("gate$o", i);
            if (!gate->isConnected()) continue;

            cModule *target = gate->getPathEndGate()->getOwnerModule();
            if (!target) continue;

            connectedPeers.push_back(i);  // Store gate index
        }

        // Choose up to 3 random peers to contact
        int numToContact = std::min(3, (int)connectedPeers.size());

        // Shuffle the peers (Fisher-Yates algorithm)
        for (int i = connectedPeers.size() - 1; i > 0; i--) {
            int j = intuniform(0, i);
            std::swap(connectedPeers[i], connectedPeers[j]);
        }

        // Send to the selected peers
        for (int i = 0; i < numToContact; i++) {
            cGate *gate = this->gate("gate$o", connectedPeers[i]);
            cModule *target = gate->getPathEndGate()->getOwnerModule();
            int peerId = target->par("clientId");

            ClientMsg *copy = gossipMsg->dup();
            copy->setDestId(peerId);

            // Add small delay to avoid congestion
            double delay = uniform(0.01, 0.05);
            sendDelayed(copy, delay, gate);

            logMessage("Sent gossip update to client " + std::to_string(peerId));
        }

        delete gossipMsg;
    }
}

void Client::checkStatus() {
    // Log current status
    logMessage("STATUS: Received gossips from " + std::to_string(receivedGossips.size()) +
              "/" + std::to_string(numClients) + " clients");
    logMessage("STATUS: Completed tasks: " + std::to_string(completedTasks) +
              "/" + std::to_string(numTasks));
}

void Client::logMessage(const std::string &message) {
    // Console log
    EV << "[Client " << clientId << "] " << message << "\n";

    // File log
    if (logFile.is_open()) {
        logFile << "[" << simTime().dbl() << "] [Client " << clientId << "] "
               << message << std::endl;
    }
}

void Client::finish() {
    // Print final statistics
    logMessage("FINAL SUMMARY: Received gossips from " + std::to_string(receivedGossips.size()) +
              "/" + std::to_string(numClients) + " clients");
    logMessage("FINAL SUMMARY: Completed " + std::to_string(completedTasks) +
              "/" + std::to_string(numTasks) + " tasks");
    logMessage("FINAL SUMMARY: Total message hops: " + std::to_string(messageHops));

    // Close log file
    if (logFile.is_open()) {
        logFile.close();
    }
}
