// Aurel Strigáč <xstrig00>

#ifndef FLOW_H
#define FLOW_H

#include <string>
#include <map>
#include <tuple>
#include <pcap.h>

/**
 * @brief ID of a network connection.
 */
struct FlowID {
    std::string ip1;   // First IP address
    std::string ip2;   // Second IP address
    std::string port1; // Port associated with ip1
    std::string port2; // Port associated with ip2
    std::string proto; // Protocol used

    /**
     * @brief Custom comparison operator for custom FlowID.
     * @param other Another FlowID to compare with.
     * @return True if this FlowID is less than other, false otherwise.
     */
    bool operator<(const FlowID &other) const;
};

/**
 * @brief Structure to hold statistics for each connection throughout the duration of one interval.
 */
struct FlowStats {
    uint64_t B_tx = 0; // Bytes transmitted
    uint64_t B_rx = 0; // Bytes received
    uint64_t p_tx = 0; // Packets transmitted
    uint64_t p_rx = 0; // Packets received
};

/**
 * @brief Global map to store data flows and their stats.
 */
extern std::map<FlowID, FlowStats> flows;

/**
 * @brief Function for checking if the connection exists in either direction and updating its statistics.
 * @param tx_key FlowID for transmitted data (ip1->ip2).
 * @param rx_key FlowID for received data (ip1<-ip2).
 * @param total_len Total length of the packet.
 */
void update_flow_statistics(const FlowID &tx_key, const FlowID &rx_key, uint16_t total_len);

/**
 * @brief Function for checking if the flow is not active.
 * @param stats Flow statistics.
 * @return True if the flow is not active, false otherwise.
 */
bool flow_not_active(const FlowStats &stats);

/**
 * @brief Function for resetting flow statistics.
 */
void reset_flow_statistics();

/**
 * @brief Function for deleting inactive connections.
 */
void trim_flows();

#endif // FLOW_H
