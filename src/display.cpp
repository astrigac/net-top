// Aurel Strigáč <xstrig00>

#include <ncurses.h>
#include <vector>
#include <algorithm>

#include "display.h"
#include "flow.h"
#include "utils.h"
#include "isa-top.h"

/**
 * @brief Function for displaying the collected statistics using ncurses.
 */
void display_statistics() {

    trim_flows();

    // Vector for holding data flows used then for sorting and displaying
    std::vector<std::pair<FlowID, FlowStats>> vec(flows.begin(), flows.end());

    sort_flows(vec);

    clear(); // Clear terminal

    display_header();

    int row = 3;    // Starting row for network statistics
    int count = 0;  // Number of currently displayed flows
    for (const auto &entry : vec) {
        if (count++ >= 10) break;   // Display only top 10 flows

        display_flow(entry.first, entry.second, row);
    }

    refresh(); // Refresh terminal

    reset_flow_statistics();
}

/**
 * @brief Function for printing the header of the statistics table.
 */
void display_header() {
    mvprintw(0, 0, "|                                    |                                    |       |         Rx        |         Tx        |");
    mvprintw(1, 0, "| %-34s | %-34s | %-5s | %-7s | %-7s | %-7s | %-7s |",
             "Src IP:port", "Dst IP:port", "Proto", "b/s", "p/s", "b/s", "p/s");
    mvprintw(2, 0, "+------------------------------------+------------------------------------+-------+---------+---------+---------+---------+");
}

/**
 * @brief Function for displaying the startup message.
 */
void display_startup() {
    mvprintw(0, 0, "LOADING...");
    refresh();
}

/**
 * @brief Function for printing a single flow's statistics.
 * @param key FlowID.
 * @param stats FlowStats.
 * @param row Current row in the display.
 */
void display_flow(const FlowID &key, const FlowStats &stats, int &row) {
    // Calculate and format values of b/s and p/s
    std::string rx_bits_str = format_bits(static_cast<double>((stats.B_rx * 8)) / refresh_interval);
    std::string tx_bits_str = format_bits(static_cast<double>((stats.B_tx * 8)) / refresh_interval);
    std::string rx_packets_str = format_packets(static_cast<double>(stats.p_rx) / refresh_interval);
    std::string tx_packets_str = format_packets(static_cast<double>(stats.p_tx) / refresh_interval);

    // Format IP addresses
    std::string ip1 = format_ip(key.ip1);
    std::string ip2 = format_ip(key.ip2);

    // Print the data for this connection
    if (key.proto == "icmp" || key.proto == "icmp6") {
        // ICMP connections should not include port numbers
        mvprintw(row++, 0, "| %-34s | %-34s | %-5s | %-7s | %-7s | %-7s | %-7s |",
                 ip1.c_str(), ip2.c_str(), key.proto.c_str(),
                 rx_bits_str.c_str(), rx_packets_str.c_str(),
                 tx_bits_str.c_str(), tx_packets_str.c_str());
    } else {
        mvprintw(row++, 0, "| %-34s | %-34s | %-5s | %-7s | %-7s | %-7s | %-7s |",
                 (ip1 + ":" + key.port1).c_str(), (ip2 + ":" + key.port2).c_str(), key.proto.c_str(),
                 rx_bits_str.c_str(), rx_packets_str.c_str(),
                 tx_bits_str.c_str(), tx_packets_str.c_str());
    }
}

/**
 * @brief Function to sort flows based on the chosen sort order.
 * @param vec Vector of flow entries to sort.
 */
void sort_flows(std::vector<std::pair<FlowID, FlowStats>> &vec) {
    if (sort_order == 'b') {
        // Sort by total bytes in the interval from highest to lowest
        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
            return (a.second.B_tx + a.second.B_rx) > (b.second.B_tx + b.second.B_rx);
        });
    } else if (sort_order == 'p') {
        // Sort by total packets in the interval from highest to lowest
        std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
            return (a.second.p_tx + a.second.p_rx) > (b.second.p_tx + b.second.p_rx);
        });
    }
}
