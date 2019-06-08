#include <bits/stdc++.h>

std::vector<std::string> split(const std::string &s, const char &delim) {
    std::vector<std::string> tokens;
    std::string cur;
    for (char c : s) {
        if (c == delim) { tokens.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}

bool startsWith(const std::string &test, const std::vector<std::string> &check) {
    for (auto &&s : check) if (test.substr(0, int(s.size())) == s) return true;
    return false;
}

std::string buyBond(int id, int price, int size) {
    return std::string("ADD ") + std::to_string(id) + " BOND BUY " + std::to_string(price) + std::string(" ") + std::to_string(size);
}

std::string sellBond(int id, int price, int size) {
    return std::string("ADD ") + std::to_string(id) + " BOND SELL " + std::to_string(price) + std::string(" ") + std::to_string(size);
}
