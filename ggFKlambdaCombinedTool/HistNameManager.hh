#ifndef _HISTNAMEMANAGER_HH
#define _HISTNAMEMANAGER_HH

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TList.h"
#include "TDirectory.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TLegend.h"
/* Usage: HistNameManager hnm = HistNameManager();
        hnm.SetDefaultBaseName();
        hnm.SetDefaultRegions();
        hnm.SetDefaultSamples();
        hnm.SetDefaultVariables();
        hnm.SetDirName("Preselection");

*/

using std::clog;
using std::endl;
using std::map;
using std::pair;
using std::string;

class HistNameManager
{

private:
    // containers for samples, regions, variables names
    vector<string> m_vec_samples;
    vector<string> m_vec_regions;
    vector<string> m_vec_variables;
    // contaniers for base samples name in inputfile
    map<string, string> m_map_basenames;
    // containers for directory name
    string m_str_directoryname;

public:
    HistNameManager();
    ~HistNameManager();

    // Set Methods
    void SetDirName(string directoryname);
    // Add method
    void AddSample(string sample);
    void AddRegion(string region);
    void AddVariable(string variable);

    // Add basenames map
    // 0p0 for hhttbbKL0p0from1p0, 1p0 for hhttbbKL1p0, 20p0 for hhttbbKL20p0from10p0
    void AddBasename(string tag, string base_name);

    // Get method
    const vector<string> &GetSamples() const { return m_vec_samples; }
    const vector<string> &GetRegions() const { return m_vec_regions; }
    const vector<string> &GetVariables() const { return m_vec_variables; }
    const map<string, string> &GetBaseMap() const { return m_map_basenames; }
    const string &GetDirName() const { return m_str_directoryname; }

    // Set default method
    void SetDefaultRegions();
    void SetDefaultVariables();
    void SetDefaultBaseName();
    
    void SetDefaultPNNVariables();
};

#endif