#include "FPGrowth.h"

#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <boost/algorithm/string.hpp>

#define PROGNAME argv[0]

using std::string;
using std::vector;

static const option long_options[] = {
    {"db",   required_argument, nullptr, 'd'},
    {"reserve", no_argument,    nullptr, 'r'},
    {"help", no_argument,       nullptr, 'h'},
    {nullptr,   0,              nullptr,  0},
};

static void usage(const string &filename) {
    std::cerr << filename << " [option]...\n"
        << "  -d|--db [Path]    Path of DB file.\n"
           "  -r|--reserve      Reserve the shor pattern(default false)\n"
           "  -h|--help         Print this help message.\n"
        << std::ends;
}

vector<string> splitString(const string &strInput, const string &delims) {
    vector<string> vecSegment;
    boost::split(vecSegment, strInput, boost::is_any_of(delims));
    return vecSegment;
}


template<typename Key, typename Value, typename Function>
void insertOrUpdate(std::map<Key, Value> &dict, const Key &key, const Value &default_, const Function &f){
    auto notFound = dict.end();
    if ( dict.find(key) == notFound )
        dict[key] = default_;
    else
        f(dict[key]);
}


int main(int argc, char *argv[]) {
    if ( argc==1 ) {
        usage(PROGNAME);
        return 2;
    }

    int opt = 0;
    int options_index = 0;

    string str_pathDBFile;
    bool reserveShortPattern=false;

    while ( (opt=getopt_long(argc, argv, "d:hr", long_options, &options_index))!=EOF ) {
        switch(opt) {
            case 0: break;
            case 'd': str_pathDBFile = optarg; break;
            case 'r': reserveShortPattern = true; break;
            case 'h':
            case '?':
            case ':': usage(PROGNAME); return 2;
        }
    }
    if ( str_pathDBFile.empty() ){
        usage(PROGNAME);
        return 2;
    }

    std::ifstream fs;
    fs.open(str_pathDBFile, std::fstream::in);
    std::string line;
    std::map<string, size_t> freqTable;
    size_t line_no = 0;
    // Scan database for creating frequent table
    for ( ;std::getline(fs, line); ++line_no ) {
        const vector<string> splited = splitString(line, ",");
        const vector<string> areas = splitString(splited[4], ":");
        const string &ip = splited[2];
        const string &client = splited[3];
        const string &country = areas[0];
        const string &province = areas[1];

        auto update_op = [](size_t &x) { ++x; };
        if ( !ip.empty() ) insertOrUpdate(freqTable, ip, size_t(1), update_op);
        if ( !client.empty() ) insertOrUpdate(freqTable, client, size_t(1), update_op);
        if ( !country.empty() ) insertOrUpdate(freqTable, country, size_t(1), update_op);
        if ( !province.empty() ) insertOrUpdate(freqTable, province, size_t(1), update_op);
        //if ( !city.empty() ) insertOrUpdate(freqTable, city, 1, update_op);
    }
    NAMESPACE_ALPHALAB_NAME::FPGrowth fpGrowth(line_no, 0.09, 0.5);
    fpGrowth.buildFPTable(freqTable);

    fs.clear();
    fs.seekg(0, fs.beg);
    // Scan database for building FP tree
    while ( std::getline(fs, line) ) {
        const vector<string> splited = splitString(line, ",");
        const vector<string> areas = splitString(splited[4], ":");
        const string &ip = splited[2];
        const string &client = splited[3];
        const string &country = areas[0];
        const string &province = areas[1];

        fpGrowth.addRecord({ip, client, country, province});
    }
    fs.close();
    std::vector<NAMESPACE_ALPHALAB_NAME::FPGrowth::TFreqPattern> patterns = fpGrowth.findAllFreqPattern(reserveShortPattern);
    for ( const auto &pattern: patterns ) {
        for ( const auto &item: pattern) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}
