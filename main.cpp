#include <iostream>
#include <string>

// Extract one valid IPv4 address, optionally followed by a port, from noisy text.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    const std::size_t n = str.size();
    std::size_t i = 0;
    while (i < n) {
        // A candidate begins at a digit and extends through the complete run
        // of digits, periods, and colons. Never accept a valid substring of it.
        if (str[i] < '0' || str[i] > '9') {
            ++i;
            continue;
        }
        const std::size_t start = i;
        while (i < n && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == ':')) {
            ++i;
        }
        const std::size_t end = i;

        // Periods/colons directly adjoining a digit-started candidate mean
        // the candidate token was split by a non-token character; reject it.
        if ((start > 0 && (str[start - 1] == '.' || str[start - 1] == ':')) ||
            (end < n && (str[end] == '.' || str[end] == ':'))) {
            continue;
        }

        std::size_t p = start;
        unsigned int octets[4] = {0, 0, 0, 0};
        bool valid = true;
        for (int part = 0; part < 4; ++part) {
            const std::size_t digitStart = p;
            unsigned int value = 0;
            while (p < end && str[p] >= '0' && str[p] <= '9') {
                if (p - digitStart >= 3) { valid = false; break; }
                value = value * 10 + static_cast<unsigned int>(str[p] - '0');
                ++p;
            }
            const std::size_t digits = p - digitStart;
            if (!valid || digits == 0 || (digits > 1 && str[digitStart] == '0') || value > 255) {
                valid = false;
                break;
            }
            octets[part] = value;
            if (part < 3) {
                if (p >= end || str[p] != '.') { valid = false; break; }
                ++p;
            }
        }

        int port = -1;
        if (valid && p < end) {
            if (str[p] != ':') valid = false;
            else {
                ++p;
                const std::size_t portStart = p;
                unsigned int portValue = 0;
                while (p < end && str[p] >= '0' && str[p] <= '9') {
                    if (p - portStart >= 5) { valid = false; break; }
                    portValue = portValue * 10 + static_cast<unsigned int>(str[p] - '0');
                    ++p;
                }
                const std::size_t digits = p - portStart;
                if (!valid || digits == 0 || (digits > 1 && str[portStart] == '0') || portValue > 65535 || p != end) {
                    valid = false;
                } else {
                    port = static_cast<int>(portValue);
                }
            }
        }
        if (valid && p == end) {
            outAddress = (static_cast<unsigned long>(octets[0]) << 24) |
                         (static_cast<unsigned long>(octets[1]) << 16) |
                         (static_cast<unsigned long>(octets[2]) << 8) |
                         static_cast<unsigned long>(octets[3]);
            outPort = port;
            return true;
        }
    }
    return false;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        if (!std::getline(std::cin, line)) break;
        if (line == "END") {
            std::cout << "Program terminated." << std::endl;
            return 0;
        }

        unsigned long address = 0;
        int port = -1;
        if (!extractIPv4(line, address, port)) {
            std::cout << "Invalid input: no valid IPv4 address found" << std::endl;
            continue;
        }

        const unsigned long a = (address >> 24) & 255UL;
        const unsigned long b = (address >> 16) & 255UL;
        const unsigned long c = (address >> 8) & 255UL;
        const unsigned long d = address & 255UL;
        std::cout << "Extracted IPv4 address: " << a << '.' << b << '.' << c << '.' << d
                  << " (decimal value: " << address << ", port: ";
        if (port < 0) std::cout << "none";
        else std::cout << port;
        std::cout << ')' << std::endl;
    }
    return 0;
}