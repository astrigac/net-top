// Aurel Strigáč <xstrig00>

#ifndef NET_TOP_H
#define NET_TOP_H

#include <string>
#include <pcap.h>

/**
 * @brief Global variables.
 */
extern char sort_order;          // Sorting order: 'b' for bytes, 'p' for packets
extern int refresh_interval;     // Output update interval in seconds
extern std::string interface;    // Network interface to capture packets from
extern pcap_t *handle;           // Pcap handle for packet capturing
extern char errbuf[];            // Buffer for pcap error messages

/**
 * @brief Function for handling the SIGINT signal.
 * @param sig Signal number.
 */
void signal_handler(int sig);

#endif // NET_TOP_H
