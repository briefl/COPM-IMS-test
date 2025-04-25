#include <iostream>
#include <filesystem>
#include <csignal>
#include <atomic>
#include "archiver.h"

std::atomic<bool> g_shouldStop{false};

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cerr << "\nReceived SIGINT. Exiting gracefully...\n";
        g_shouldStop = true;
    }
}

int print_usage() {
    std::cerr << "Usage:\n"
              << "  my-project a <input_file> <output_file>   Compress file\n"
              << "  my-project e <input_file> <output_file>   Decompress file\n";
    return 1;
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, signal_handler);

    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments.\n";
        return print_usage();
    }

    std::string mode = argv[1];
    std::string inFile = argv[2];
    std::string outFile = argv[3];

    if (!std::filesystem::exists(inFile)) {
        std::cerr << "Error: Input file does not exist: " << inFile << "\n";
        return 2;
    }

    if (g_shouldStop) {
        std::cerr << "Aborted before operation started.\n";
        return 130; // 128 + SIGINT
    }

    int result = 0;

    if (mode == "a") {
        result = compress_file(inFile.c_str(), outFile.c_str());
    } else if (mode == "e") {
        result = decompress_file(inFile.c_str(), outFile.c_str());
    } else {
        std::cerr << "Error: Unknown mode '" << mode << "'.\n";
        return print_usage();
    }

    if (g_shouldStop) {
        std::cerr << "Operation interrupted by user.\n";
        return 130;
    }

    if (result != 0) {
        std::cerr << "Operation failed with error code: " << result << "\n";
        return result;
    }

    std::cout << "Operation completed successfully.\n";
    return 0;
}
