// Aurel Strigáč <xstrig00>

#include <iostream>
#include <string>
#include <pcap.h>
#include <cstring>
#include <csignal>
#include <chrono>
#include <thread>
#include <cstdlib>

#include <ncurses.h>

#include "utils.h"
#include "display.h"
#include "flow.h"
#include "capture.h"
#include "isa-top.h"

std::string interface;                              // Network interface to capture packets from
char sort_order = 'b';                              // Sorting order: 'b' for bytes, 'p' for packets
int refresh_interval = 1;                           // Output update interval in seconds
pcap_t *handle = nullptr;                           // Pcap handle for packet capturing
char errbuf[PCAP_ERRBUF_SIZE];                      // Buffer for pcap error messages

/**
 * @brief Function for handling the SIGINT signal.
 * @param sig Signal number.
 */
void signal_handler(int sig) {
    (void)sig; // Pity fix for unused variable

    endwin();
    pcap_close(handle);

    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);

    parse_args(argc, argv);
    
    // Initialization of packet capture on the interface
    if ((handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf)) == nullptr) {
        std::cerr << "[ ERROR ] Cannot open device " << interface << ": " << errbuf << "\n";
        return EXIT_FAILURE;
    }

    // Set the pcap handle to non-blocking mode
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        std::cerr << "[ ERROR ] Cannot set non-blocking mode: " << errbuf << "\n";
        return EXIT_FAILURE;
    }

    // Validation that the provided interface supports ethernet packets
    check_ethernet_support();

    auto start_time = std::chrono::steady_clock::now();

    // Initialation of ncurses
    initscr();
    noecho();
    cbreak();

    display_startup(); 

    while (true) {
        // Process incomming and outgoing network traffic
        pcap_dispatch(handle, -1, packet_handler, nullptr);


        auto curr_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> last_refresh = curr_time - start_time;

        // Check, if we have reached referesh interval
        if (last_refresh.count() >= refresh_interval) {
            start_time = curr_time;
            display_statistics();
        }
    }

    endwin();
    pcap_close(handle);

    return 0;
}
