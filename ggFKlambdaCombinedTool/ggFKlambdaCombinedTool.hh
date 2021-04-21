#ifndef _COMOBINEDTOOL_HH
#define _COMOBINEDTOOL_HH
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
class ggFKlambdaCombinedTool
{

    using std::map;
    using std::string;
    using std::vector;

private:
    // input & output file
    TFile *m_input_file;
    TFile *m_output_file;

    // Base and combined histogram
    TH1F *m_h0;
    TH1F *m_h1;
    TH1F *m_h20;
    TH1F *m_combined;

    // Get base hists and check them
    void GetHistAndCheck();

public:
    // Constructor
    // default
    ggFKlambdaCombinedTool();
    // deconstructor
    - ggFKlambdaCombinedTool();
    // construct really use
    ggFKlambdaCombinedTool(const string &input_file, const string &output_file);

    // Get Methods

    // Get Combined Hist
    TH1F *GetCombinedHist();

    // Set Methods

    // Combine and write to output file
    void CombineAndWrite(float klambda);

    // Useful toolkit, transform such 1.0 to "1p0", -1.0 to "n1p0"
    std::string transformFloat(float KLambda);
};

#endif