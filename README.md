# IPv4 Extractor

A C++ program that scans noisy text and extracts the first valid IPv4 address.

## Build and run

```sh
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o ipv4_extractor
./ipv4_extractor
```

Enter one line at a time. Enter `END` by itself to exit. See `tests/test_cases.txt` for the cases used during validation. The AI use record is in `AI_DISCLOSURE.md`.
