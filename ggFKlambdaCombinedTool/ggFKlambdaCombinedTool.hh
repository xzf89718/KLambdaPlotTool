#ifndef _COMOBINEDTOOL_HH
#define _COMOBINEDTOOL_HH

/*
Usage: 
ggFKlambdaCombinedTool combinetool(input_file, output_file);
combinetool.SetDefaultHistNameManager();
combinetool.LoopOverHistNameManagers(1.0);
*/
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

#include "HistNameManager.hh"
using std::map;
using std::string;
using std::vector;

class ggFKlambdaCombinedTool
{

private:
    // input & output file
    TFile *m_input_file;
    TFile *m_output_file;

    // input & output directory
    TDirectory *m_input_dir;
    TDirectory *m_output_dir;

    // debug
    bool m_debug;
    // Base and combined histogram
    TH1F *m_h0;
    TH1F *m_h1;
    TH1F *m_h20;
    TH1F *m_combined;

        
    // reweighted hist name
    string m_reweighted_hist_name;
    // contaniner for HistNameManager, manage histname!
    vector<HistNameManager *> m_vec_HistNameManagers;

    // Combine and write to output file
    void CombineOneHist(float klambda);

    // Delete base and combined histrograms
    void DeleteHists();
    // Draw all plots!
    // void DrawDebugPlots();

    // Get base hists and check them
    bool GetHistAndCheck(const string &h0_name, const string &h1_name, const string &h20_name);

public:
    // Constructor
    // default
    ggFKlambdaCombinedTool();
    // deconstructor
    ~ggFKlambdaCombinedTool();
    // construct really use
    ggFKlambdaCombinedTool(const string &input_file, const string &output_file);

    // Add HistNameManager
    void AddHistNameManager(HistNameManager *hnm) { m_vec_HistNameManagers.push_back(hnm); };

    // Get Methods

    // // Get Combined Hist
    // TH1F *GetCombinedHist() { return m_combined; };

    // Loop over all things in m_vec_HistNameManagers
    void LoopOverHistNameManagers(float klambda);
    // Set Methods
    // Set debug
    void SetDebug(bool debug_or_not);
    // Set default setup
    void SetDefaultHistNameManager();

    // Useful toolkit, transform such 1.0 to "1p0", -1.0 to "n1p0"
    std::string TransformFloat(float KLambda);
};

#endif