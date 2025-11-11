// Aurel Strigáč <xstrig00>

#ifndef DISPLAY_H
#define DISPLAY_H

#include "flow.h"
#include <vector>

/**
 * @brief Function for displaying the collected statistics using ncurses.
 */
void display_statistics();

/**
 * @brief Function for printing the header of the statistics table.
 */
void display_header();

/**
 * @brief Function for displaying the startup message.
 */
void display_startup();

/**
 * @brief Function for printing a single flow's statistics.
 * @param key FlowID.
 * @param stats FlowStats.
 * @param row Current row in the display.
 */
void display_flow(const FlowID &key, const FlowStats &stats, int &row);

/**
 * @brief Function to sort flows based on the chosen sort order.
 * @param vec Vector of flow entries to sort.
 */
void sort_flows(std::vector<std::pair<FlowID, FlowStats>> &vec);

#endif // DISPLAY_H
