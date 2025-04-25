#include "archiver.h"
#include <zlib.h>
#include <fstream>
#include <iostream>
#include <atomic>
#include <csignal>

constexpr size_t CHUNK = 16384;

extern std::atomic<bool> g_shouldStop;

int compress_file(const char* inFilename, const char* outFilename) {
    std::ifstream in(inFilename, std::ios::binary);
    if (!in) {
        std::cerr << "Error: Cannot open input file for reading: " << inFilename << "\n";
        return 1;
    }

    gzFile out = gzopen(outFilename, "wb");
    if (!out) {
        std::cerr << "Error: Cannot open output file for writing (gzopen failed): " << outFilename << "\n";
        return 2;
    }

    char buffer[CHUNK];
    while (!g_shouldStop && (in.read(buffer, CHUNK) || in.gcount())) {
        if (gzwrite(out, buffer, in.gcount()) == 0) {
            std::cerr << "Error: Failed to write to gzip file.\n";
            gzclose(out);
            return 3;
        }
    }

    gzclose(out);

    if (g_shouldStop) {
        std::cerr << "Compression interrupted by user.\n";
        return 4;
    }

    return 0;
}

bool is_gzip(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    unsigned char byte1 = 0, byte2 = 0;
    file.read(reinterpret_cast<char*>(&byte1), 1);
    file.read(reinterpret_cast<char*>(&byte2), 1);

    return byte1 == 0x1f && byte2 == 0x8b;
}

int decompress_file(const char* inFilename, const char* outFilename) {
    if (!is_gzip(inFilename)) {
        std::cerr << "Error: File does not appear to be in gzip format.\n";
        return 6;
    }

    gzFile in = gzopen(inFilename, "rb");
    if (!in) {
        std::cerr << "Error: Cannot open input gzip file: " << inFilename << "\n";
        return 1;
    }

    std::ofstream out(outFilename, std::ios::binary);
    if (!out) {
        std::cerr << "Error: Cannot open output file for writing: " << outFilename << "\n";
        gzclose(in);
        return 2;
    }

    char buffer[CHUNK];
    int bytes;
    bool hasData = false;

    while (!g_shouldStop && (bytes = gzread(in, buffer, CHUNK)) > 0) {
        hasData = true;
        out.write(buffer, bytes);
        if (!out) {
            std::cerr << "Error: Failed to write to output file.\n";
            gzclose(in);
            return 3;
        }
    }

    gzclose(in);

    if (g_shouldStop) {
        std::cerr << "Decompression interrupted by user.\n";
        return 4;
    }

    if (!hasData) {
        std::cerr << "Error: No data was read. Possibly empty or corrupted gzip.\n";
        return 5;
    }

    return 0;
}
