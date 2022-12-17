#pragma once
#include <string_view>
#include <string>
#include <ctime>

// Constants used to print in color to the command line
// Taken from https://stackoverflow.com/a/9158263
#define RESET       "\033[0m"
#define BLACK       "\033[30m"             // Black
#define RED         "\033[31m"             // Red
#define GREEN       "\033[32m"             // Green
#define YELLOW      "\033[33m"             // Yellow
#define BLUE        "\033[34m"             // Blue
#define MAGENTA     "\033[35m"             // Magenta
#define CYAN        "\033[36m"             // Cyan
#define WHITE       "\033[37m"             // White
#define BOLDBLACK   "\033[1m\033[30m"      // Bold Black
#define BOLDRED     "\033[1m\033[31m"      // Bold Red
#define BOLDGREEN   "\033[1m\033[32m"      // Bold Green
#define BOLDYELLOW  "\033[1m\033[33m"      // Bold Yellow
#define BOLDBLUE    "\033[1m\033[34m"      // Bold Blue
#define BOLDMAGENTA "\033[1m\033[35m"      // Bold Magenta
#define BOLDCYAN    "\033[1m\033[36m"      // Bold Cyan
#define BOLDWHITE   "\033[1m\033[37m"      // Bold White

/**
 * @brief Computes the time elapsed between the two given clock cycle counts and
 * prints the CPU time in seconds along with the name of the associated action.
 * 
 * @param c1 a clock_t indicating the clock at the start of the action.
 * @param c2 a clock_t indicating the clock at the end of the action.
 * @param action a string indicating the name of the action associated with the 
 * given clocks and computed elapsed time.
 */
void print_elapsed(clock_t& c1, clock_t& c2, const std::string_view& action);

/**
 * @brief Compare two pairs representing betweenness centrality scores for two
 * different vertices denoted by the string indicating the vertex address in the
 * blockchain and the betweenness centrality score of the address in the 
 * network. This function is used to sort an unordered map of betweenness 
 * centrality scores using a vector.
 * 
 * @param p1 one of the pairs composed of a string and a double used to compare.
 * @param p2 another pair composed of a string and a double used to compare.
 * @return true if the betweenness centrality score of the first pair is larger
 * than the betweenness centrality score of the second pair.
 * @return false if the betweenness centrality score of the first pair is 
 * smaller than the betweenness centrality score of the second pair.
 */
bool compare_bc_pair(const std::pair<std::string, double>& p1,
                     const std::pair<std::string, double>& p2);
