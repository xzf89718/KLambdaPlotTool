#ifndef _COMPAREHISTMANAGER_HH
#define _COMPAREHISTMANAGER_HH
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "HistNameManager.hh"

using std::map;
using std::pair;
using std::string;

class CompareHistManager : public HistNameManager
{
private:
    // string m_str_basename_1;
    // string m_str_basename_2;

    // rebin factor
    map<string, int> m_map_rebin_factor;

public:
    // Constructor!
    CompareHistManager();
    ~CompareHistManager();
    // const string &GetBaseName1() const;
    // const string &GetBaseName2() const;
    // void SetBaseName1(string basename);
    // void SetBaseName2(string basename);
    void AddRebinFactor(const string &var_name, const int &rebin_factor);
    const map<string, int> &GetRebinFactor() { return m_map_rebin_factor; }
    void SetDefaultRebinFactors();
};

#endif