#ifndef _COMPAREHISTMANAGER_HH
#define _COMPAREHISTMANAGER_HH
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "HistNameManager.hh"

class CompareHistManager : public HistNameManager
{
private:
    m_str_basename_1;
    m_str_basename_2;

public:
    void SetBaseName1(string &basename);
    void SetBaseName2(string &basename);

    const string &GetBaseName1() const {return m_str_basename_1};
    const string &GetBaseName2() const {return m_str_basename_2};
}

#endif