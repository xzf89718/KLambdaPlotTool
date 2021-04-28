#ifndef _COMPAREPLOT_HH
#define _COMPAREPLOT_HH
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

#include "TStyle.h"
#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TList.h"


#include "CompareHistManager.hh"

using std::string;
using std::vector;

class ComparePlot
{
private:
    // input files and directories!
    TFile *m_input_file1;
    TFile *m_input_file2;
    TDirectory *m_input_directory1;
    TDirectory *m_input_directory2;

    TFile *m_output_file;
    TDirectory *m_output_dir;

    // TCanvas Name!
    string *m_canvas_name;

    // TH1F
    TH1F *m_h1;
    TH1F *m_h2;
    TH1F *m_h3;
    // TCanvas    
    TCanvas *m_c1;
    // TPad
    TPad *m_pad1;
    TPad *m_pad2;
    // TLegend
    TLegend *m_legend;
    // Rebin factor
    int m_rebin_factor;    

    // Container for CompareHistManager
    vector<CompareHistManager *> m_vec_CompareHistManager;

    // Delete m_h1, m_h2, and m_c1
    void DeleteHistsAndCanvas();
    bool GetHistAndCheck(const string &h1_name, const string &h2_name);
    void PlotRatio();

public:
    ComparePlot();
    ~ComparePlot();

    ComparePlot(const string &input_file1_name, const string &input_file2_name, const string &output_file_name);

    void AddCompareHistManager(CompareHistManager *chm);

    void LoopOverCompareHistManager(float klambda);

    void SetDefaultCompareHistManager();

    string TransformFloat(float KLambda);
};
#endif