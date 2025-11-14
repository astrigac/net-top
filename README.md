# net-top: Network Traffic Monitor

`net-top` is a command-line utility designed to capture and analyze network traffic on a specified interface. It provides real-time statistics about network flows, similar to the functionality of the well-known tool `iftop`. The application identifies IP addresses, port numbers, and transport protocols to monitor data flow and displays dynamically updated statistics in the terminal.

![showcase](https://github.com/user-attachments/assets/f68c333e-4770-43c9-827a-e02c9cd2cd0b)

This project was developed as part of the Network Applications and Network Administration (ISA) course at the Faculty of Information Technology, Brno University of Technology.

## Language and Libraries

*   **Language:** C++
*   **Packet Capture:** `libpcap`
*   **Terminal UI:** `ncurses`
*   **Build System:** `Makefile`

## File Structure

```
.
├── include/
│   ├── capture.h       # Header for packet capturing and parsing
│   ├── display.h       # Header for UI and ncurses functions
│   ├── flow.h          # Header for network flow data structures
│   ├── net-top.h       # Main application header
│   └── utils.h         # Header for utility functions (argument parsing, formatting)
├── src/
│   ├── capture.cpp     # Implements packet capturing and L3/L4 parsing
│   ├── display.cpp     # Implements the ncurses display logic
│   ├── flow.cpp        # Implements network flow management
│   ├── net-top.cpp     # Main application logic (main loop, pcap/ncurses init)
│   └── utils.cpp       # Implements utility and helper functions
├── tests/              # (Optional) Directory for tests
├── .gitignore          # Git ignore file
├── net-top.1           # Man page
├── LICENSE             # Project license
├── Makefile            # Build script for compiling the project
├── manual.pdf          # PDF documentation (in Slovak)
└── README.md           # This file```

## Getting Started

Follow these instructions to compile and run the application on a Linux-based system.

### Step 1: Install Prerequisites

Before compiling, you need to install the necessary libraries:
```bash
sudo apt update
sudo apt install build-essential libpcap-dev libncurses5-dev libncursesw5-dev
```

### Step 2: Compile the Program

Clone the repository and go to the project directory. Then, use the provided `Makefile` to build the executable.

1.  **Navigate to the source directory:**
    ```bash
    cd /path/to/net-top
    ```

2.  **Compile the project:**
    ```bash
    make
    ```
    This will create an executable file named `net-top` in the current directory.

3.  **(Optional) Clean build files:**
    To remove the generated object files and the executable, you can run:
    ```bash
    make clean
    ```

### Step 3: Run the Application

To run the program, you must specify a network interface to monitor. You may need to run the command with `sudo`.

**Basic command structure:**
```bash
sudo ./net-top -i <interface-id> [-s b|p] [-t <seconds>] [-h|--help]
```

#### Command-Line Parameters

*   `-i <interface-id>`: **(Required)** Specifies the network interface to monitor (e.g., `eth0`, `wlan0`).
*   `-s [b|p]`: **(Optional)** Sets the sorting criteria for network flows.
    *   `b`: Sort by total bytes transferred (default).
    *   `p`: Sort by total packets transferred.
*   `-t <seconds>`: **(Optional)** Sets the statistics refresh interval in seconds. Must be greater than 0. The default is 1 second.
*   `-h` or `--help`: Displays the help message and exits.

### Usage Examples

1.  **Monitor traffic on `wlan0` with a 2-second refresh interval:**
    ```bash
    sudo ./net-top -i wlan0 -t 2
    ```

2.  **Monitor traffic on `enp0s3`, sorting by packet count with a 5-second interval:**
    ```bash
    sudo ./net-top -i enp0s3 -s p -t 5
    ```

3.  **Display the help message:**
    ```bash
    ./net-top --help
    ```
