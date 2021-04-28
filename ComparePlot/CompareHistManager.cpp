#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "CompareHistManager.hh"

using std::clog;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;

CompareHistManager::CompareHistManager() : HistNameManager()
{
    // m_str_basename_1 = string();
    // m_str_basename_2 = string();
    m_map_rebin_factor = map<string, int>();
    cout << "CompareHistManager::CompareHistManager: INFO Default Initialize" << endl;
}

CompareHistManager::~CompareHistManager()
{
    cout << "CompareHistManager::~CompareHistManager: INFO Deleted" << endl;
}

void CompareHistManager::AddRebinFactor(const string &var_name, const int &rebin_factor)
{

    auto factor = pair<string, int>(var_name, rebin_factor);
    auto isInsertSuccess = m_map_rebin_factor.insert(factor);
    if (isInsertSuccess.second == false)
    {
        clog << "CompareHistManager::AddCompareHistManager: WARNING Duplicated pair. The key is already exist in m_map_rebin_factor" << endl;
    }
}

void CompareHistManager::SetDefaultRebinFactors()
{
    AddRebinFactor("mBB", 10);
    AddRebinFactor("mMMC", 12);
    AddRebinFactor("mHH", 12);
    AddRebinFactor("mHHScaled", 12);
    AddRebinFactor("dRBB", 4);
    AddRebinFactor("pTBB", 10);
    AddRebinFactor("dRTauTau", 4);
    AddRebinFactor("pTTauTau", 10);
    AddRebinFactor("dPhiBBTauTau", 4);
    AddRebinFactor("dRBBTauTau", 5);
    AddRebinFactor("pTBalance", 5);
    AddRebinFactor("MET", 5);
    AddRebinFactor("nJets", 1);
    AddRebinFactor("Yield", 1);
    AddRebinFactor("metSig", 2);
    AddRebinFactor("metSigPU", 2);
    AddRebinFactor("Jet0Pt", 10);
    AddRebinFactor("Jet1Pt", 10);
    AddRebinFactor("pTB0", 10);
    AddRebinFactor("pTB1", 10);
    AddRebinFactor("LeadJetPt", 10);
    AddRebinFactor("SubleadJetPt", 10);
    AddRebinFactor("LeadJetEta", 2);
    AddRebinFactor("SubleadJetEta", 2);
    AddRebinFactor("Jet0Eta", 2);
    AddRebinFactor("Jet1Eta", 2);
    AddRebinFactor("Tau0Pt", 10);
    AddRebinFactor("Tau1Pt", 10);
    AddRebinFactor("Tau0Eta", 2);
    AddRebinFactor("Tau1Eta", 2);
    AddRebinFactor("Tau0Ntrk", 1);
    AddRebinFactor("Tau1Ntrk", 1);
    AddRebinFactor("dPhiTauTau", 4);
    AddRebinFactor("PNN", 20);
    AddRebinFactor("SMBDT", 20);
    AddRebinFactor("SMNN", 20);

    cout << "CompareHistManager::SetDefaultRebinFactors: INFO Set default rebin factors" << endl;
}
// void CompareHistManager::SetBaseName1(string basename)
// {
//     m_str_basename_1 = basename;
// }

// void CompareHistManager::SetBaseName2(string basename)
// {
//     m_str_basename_2 = basename;
// }

// const string &CompareHistManager::GetBaseName1() const
// {
//     if (!m_str_basename_1.size())
//     {
//         clog << "CompareHistManager::GetBaseName1: ERROR BaseName1 not set, abort!" << endl;
//         std::abort();
//     }
//     return m_str_basename_1;
// }

// const string &CompareHistManager::GetBaseName2() const
// {
//     if (!m_str_basename_2.size())
//     {
//         clog << "CompareHistManager::GetBaseName2: ERROR BaseName2 not set, abort!" << endl;
//         std::abort();
//     }
//     return m_str_basename_2;
// }
