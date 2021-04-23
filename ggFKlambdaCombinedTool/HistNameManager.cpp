#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "HistNameManager.hh"

using std::clog;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

HistNameManager::HistNameManager()
{
    // m_vec_samples = vector<string>();
    m_vec_regions = vector<string>();
    m_vec_variables = vector<string>();
    // contaniers for base samples name in inputfile
    m_map_basenames = map<string, string>();
    // containers for directory name
    m_str_directoryname = string();

    cout << "HistNameManager: Initialize!" << endl;
}

HistNameManager::~HistNameManager()
{
    cout << "HistNameManager: Deleted!" << endl;
}

// Set Methods
void HistNameManager::SetDirName(string directoryname)
{
    m_str_directoryname = directoryname;
}

// Add method
// void HistNameManager::AddSample(string sample)
// {
//     m_vec_samples.push_back(sample);
// }

void HistNameManager::AddRegion(string region)
{
    m_vec_regions.push_back(region);
}
void HistNameManager::AddVariable(string variable)
{
    m_vec_variables.push_back(variable);
}

// Add basenames map
// 0p0 for hhttbbKL0p0from1p0, 1p0 for hhttbbKL1p0, 20p0 for hhttbbKL20p0from10p0
void HistNameManager::AddBasename(string tag, string base_name)
{
    m_map_basenames.insert(pair<string, string>(tag, base_name));
}

// Set default method
// SetDefaultSamples()
// {
// }
void HistNameManager::SetDefaultRegions()
{
    // Add region names
    AddRegion("2tag2pjet_0ptv_LL_OS");
    AddRegion("2tag2pjet_0ptv_LL_SS");
    AddRegion("1tag2pjet_0ptv_LL_OS");
    AddRegion("1tag2pjet_0ptv_LL_SS");
}
void HistNameManager::SetDefaultVariables()
{
    //Add all variables to variable_names
    AddVariable("mBB");
    AddVariable("mMMC");
    AddVariable("mHH");
    AddVariable("mHHScaled");
    AddVariable("dRBB");
    AddVariable("pTBB");
    AddVariable("dRTauTau");
    AddVariable("pTTauTau");
    AddVariable("dPhiBBTauTau");
    AddVariable("dRBBTauTau");
    AddVariable("pTBalance");
    AddVariable("MET");
    AddVariable("nJets");
    AddVariable("Yield");
    AddVariable("metSig");
    AddVariable("metSigPU");
    AddVariable("Jet0Pt");
    AddVariable("Jet1Pt");
    AddVariable("pTB0");
    AddVariable("pTB1");
    AddVariable("LeadJetPt");
    AddVariable("SubleadJetPt");
    AddVariable("LeadJetEta");
    AddVariable("SubleadJetEta");
    AddVariable("Jet0Eta");
    AddVariable("Jet1Eta");
    AddVariable("Tau0Pt");
    AddVariable("Tau1Pt");
    AddVariable("Tau0Eta");
    AddVariable("Tau1Eta");
    AddVariable("Tau0Ntrk");
    AddVariable("Tau1Ntrk");
    AddVariable("dPhiTauTau");
}
void HistNameManager::SetDefaultBaseName()
{
    AddBasename("base", "hhttbbKL");
    AddBasename("1p0", "hhttbbKL1p0");
    AddBasename("10p0", "hhttbbKL10p0");
    AddBasename("20p0", "hhttbbKL20p0from10p0");
    AddBasename("0p0", "hhttbbKL0p0from1p0");
}

void HistNameManager::SetDefaultPNNVariables()
{
    vector<string> pnn_klambdas = {"n20p0", "n15p0", "n10p0", "n5p0", "n4p0", "n3p0", "n2p0", "n1p0",
                                   "0p0", "0p4", "0p8", "1p0", "1p2", "1p6", "2p0", "2p4",
                                   "3p0", "4p0", "5p0", "10p0", "15p0", "20p0"};

    for (auto pnn_klambda = pnn_klambdas.begin(); pnn_klambda != pnn_klambdas.end(); pnn_klambda++)
    {
        AddVariable("PNN" + *pnn_klambda);
    }
}

void HistNameManager::SetDefaultBDTVariables(){
    AddVariable("SMBDT");
}

void HistNameManager::SetDefaultNNVariables(){
    AddVariable("SMNN");
    AddVariable("SMNNNonNested");
}