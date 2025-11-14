// Aurel Strigáč <xstrig00>

#include <cstdio>
#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <pcap.h>
#include <cstring>

#include "utils.h"
#include "net-top.h"

/**
 * @brief Function for formatting bit rates.
 * @param bits_per_sec Bit rate to format.
 * @return Formatted string representing the bit rate.
 */
std::string format_bits(double bits_per_sec) {
    const char *units = " KMGTP"; // Available suffixes
    int index = 0;
    char formatted_number[30];

    // Number conversion
    for (index = 0; bits_per_sec >= 1000 && index < 5; bits_per_sec /= 1000, index++) {
    }

    // Determine whether the number is a whole number and append suffix
    if (bits_per_sec == static_cast<int>(bits_per_sec)) {
        snprintf(formatted_number, sizeof(formatted_number), "%d%c", static_cast<int>(bits_per_sec), units[index]);
    } else {
        snprintf(formatted_number, sizeof(formatted_number), "%.1f%c", bits_per_sec, units[index]);
    }

    return std::string(formatted_number);
}

/**
 * @brief Function for formatting packet rates.
 * @param packets_per_sec Packet rate to format.
 * @return Formatted string representing the packet rate.
 */
std::string format_packets(double packets_per_sec) {
    const char *units = " KMG"; // Available suffixes
    int index = 0;
    char formatted_number[30];

    // Number conversion
    for (index = 0; packets_per_sec >= 1000 && index < 3; packets_per_sec /= 1000, index++) {
    }

    // Determine whether the number is a whole number and append suffix
    if (packets_per_sec == static_cast<int>(packets_per_sec)) {
        snprintf(formatted_number, sizeof(formatted_number), "%d%c", static_cast<int>(packets_per_sec), units[index]);
    } else {
        snprintf(formatted_number, sizeof(formatted_number), "%.1f%c", packets_per_sec, units[index]);
    }

    return std::string(formatted_number);
}

/**
 * @brief Function for formatting IP addresses before printing.
 * @param ip IP address string.
 * @return Formatted IP address string.
 */
std::string format_ip(const std::string &ip) {
    return ip.find(':') != std::string::npos ? "[" + ip + "]" : ip;
}

/**
 * @brief Function for displaying the help message.
 */
void print_help() {
    std::cout << "\nUSAGE:\n"
              << "./net-top -i interface-id [-s b|p] [-t seconds]\n\n"
              << "Options:\n"
              << "  -i         :  Interface on which the application listens defined by its identifier.\n"
              << "  -s         :  Sort output by:\n"
              << "                  b - bytes (default)\n"
              << "                  p - packets\n"
              << "  -t         :  Refresh interval for statistics in seconds, must be greater than 0 (default: 1).\n"
              << "  -h, --help :  Display this help message and exit.\n\n";
}

/**
 * @brief Function for checking sort order parameter.
 * @param order Sort order character.
 */
void check_sort_order(char order) {
    if (order != 'b' && order != 'p') {
        std::cerr << "[ ERROR ] Invalid -s option.\n";
        print_help();
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Function for checking refresh interval parameter.
 * @param interval Refresh interval in seconds.
 */
void check_refresh_interval(int interval) {
    if (interval <= 0) {
        std::cerr << "[ ERROR ] Invalid -t option.\n";
        print_help();
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Function for checking if the interface parameter is set.
 */
void check_interface_set() {
    if (interface.empty()) {
        std::cerr << "[ ERROR ] Network interface is required.\n";
        print_help();
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Function for handling invalid arguments.
 */
void handle_invalid_argument() {
    std::cerr << "[ ERROR ] Invalid argument(s).\n";
    print_help();
    exit(EXIT_FAILURE);
}

/**
 * @brief Function for parsing command-line arguments.
 * @param argc Argument count.
 * @param argv Argument vector.
 */
void parse_args(int argc, char *argv[]) {
    int opt;

    // Definitions of long versions of parameters
    struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    // Parsing of arguments
    while ((opt = getopt_long(argc, argv, "i:s:t:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'i':
                interface = optarg;
                break;
            case 's':
                sort_order = optarg[0];
                check_sort_order(sort_order);
                break;
            case 't':
                refresh_interval = std::atoi(optarg);
                check_refresh_interval(refresh_interval);
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            default:
                handle_invalid_argument();
        }
    }

    check_interface_set();
}

/**
 * @brief Function for checking if the interface supports Ethernet headers.
 */
void check_ethernet_support() {
    if (pcap_datalink(handle) != DLT_EN10MB) {
        std::cerr << "[ ERROR ] Device " << interface << " doesn't provide Ethernet headers.\n";
        exit(EXIT_FAILURE);
    }
}
