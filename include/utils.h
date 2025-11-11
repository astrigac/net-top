// Aurel Strigáč <xstrig00>

#ifndef UTILS_H
#define UTILS_H

#include <string>

/**
 * @brief Function for formatting bit rates.
 * @param bits_per_sec Bit rate to format.
 * @return Formatted string representing the bit rate.
 */
std::string format_bits(double bits_per_sec);

/**
 * @brief Function for formatting packet rates.
 * @param packets_per_sec Packet rate to format.
 * @return Formatted string representing the packet rate.
 */
std::string format_packets(double packets_per_sec);

/**
 * @brief Function for formatting IP addresses before printing.
 * @param ip IP address string.
 * @return Formatted IP address.
 */
std::string format_ip(const std::string &ip);

/**
 * @brief Function for displaying the help message.
 */
void print_help();

/**
 * @brief Function for checking sort order parameter.
 * @param order Sort order (b/p).
 */
void check_sort_order(char order);

/**
 * @brief Function for checking refresh interval parameter.
 * @param interval Refresh interval in seconds.
 */
void check_refresh_interval(int interval);

/**
 * @brief Function for checking if the interface parameter is set.
 */
void check_interface_set();

/**
 * @brief Function for handling invalid arguments.
 */
void handle_invalid_argument();

/**
 * @brief Function for parsing command-line arguments.
 * @param argc Argument count.
 * @param argv Argument vector.
 */
void parse_args(int argc, char *argv[]);

/**
 * @brief Function for checking if the interface supports Ethernet headers.
 */
void check_ethernet_support();

#endif // UTILS_H
