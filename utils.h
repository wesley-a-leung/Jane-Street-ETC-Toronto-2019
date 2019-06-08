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
    std::string ret = std::string("ADD ") + std::to_string(id) + " BOND BUY " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string sellBond(int id, int price, int size) {
    std::string ret = std::string("ADD ") + std::to_string(id) + " BOND SELL " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}


std::string buyStock(int id, int price, int size, string stock) {
    std::string ret = "ADD " + std::to_string(id) + " " + stock + " BUY " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string sellStock(int id, int price, int size, string stock) {
    std::string ret = "ADD " + std::to_string(id) + " " + stock + " SELL " + std::to_string(price) + std::string(" ") + std::to_string(size);
    std::cout << ret << std::endl;
    return ret;
}

std::string cancel(int id) {
    std::cout << "Cancelling:  " << id << endl;
    std::string ret = std::string("CANCEL ") + std::to_string(id);
    return ret;
}
