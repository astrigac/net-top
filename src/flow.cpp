// Aurel Strigáč <xstrig00>

#include "flow.h"
#include "isa-top.h"

std::map<FlowID, FlowStats> flows;

/**
 * @brief Custom comparison operator for custom FlowID.
 * @param other Another FlowID to compare with.
 * @return True if this FlowID is less than other, false otherwise.
 */
bool FlowID::operator<(const FlowID &other) const {
    return std::tie(ip1, ip2, port1, port2, proto) <
           std::tie(other.ip1, other.ip2, other.port1, other.port2, other.proto);
}

/**
 * @brief Function for checking if the connection exists in either direction and updating its statistics.
 * @param tx_key FlowID for transmitted data (ip1->ip2).
 * @param rx_key FlowID for received data (ip1<-ip2).
 * @param total_len Total length of the packet.
 */
void update_flow_statistics(const FlowID &tx_key, const FlowID &rx_key, uint16_t total_len) {
    auto tx_item = flows.find(tx_key);
    auto rx_item = flows.find(rx_key);

    if (tx_item != flows.end()) {
        // Connection already exists in the SrcIP->DstIP direction, so we are transmitting
        tx_item->second.B_tx += total_len;
        tx_item->second.p_tx += 1;
    } else if (rx_item != flows.end()) {
        // Connection already exists in the DstIP->SrcIP direction, so we are receiving
        rx_item->second.B_rx += total_len;
        rx_item->second.p_rx += 1;
    } else {
        // Connection doesn't exist in either direction
        flows[tx_key] = {total_len, 0, 1, 0};
    }
}

/**
 * @brief Function for checking if the flow is not active.
 * @param stats Flow statistics.
 * @return True if the flow is not active, false otherwise.
 */
bool flow_not_active(const FlowStats &stats) {
    return stats.B_tx == 0 && stats.B_rx == 0 && stats.p_tx == 0 && stats.p_rx == 0;
}

/**
 * @brief Function for resetting flow statistics.
 */
void reset_flow_statistics() {
    for (auto flow = flows.begin(); flow != flows.end(); flow++) {
        flow->second.B_tx = 0;
        flow->second.B_rx = 0;
        flow->second.p_tx = 0;
        flow->second.p_rx = 0;
    }
}

/**
 * @brief Function for deleting inactive connections.
 */
void trim_flows() {
    for (auto flow = flows.begin(); flow != flows.end();) {
        if (flow_not_active(flow->second)) {
            // Remove non-active flows
            flow = flows.erase(flow);
        } else {
            flow++;
        }
    }
}
