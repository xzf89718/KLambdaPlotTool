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

class ggFKlambdaCombinedTool
{

    using std::vector;
    using std::string;
    using std::map;

private:
    // input & output file
    TFile* m_input_file;
    TFile* m_output_file;

    // containers for samples, regions, variables names
    vector<string>* m_vec_samples;
    vector<string>* m_vec_regions;
    vector<string>* m_vec_preselection_variables;
    vector<string>* m_vec_bdtscorepreselection_variables;
    vector<string>* m_vec_pnnscorepreselection_variables;
    // contaniers for base samples name in inputfile
    vector<string>* m_vec_basenames;
    // containers for rebin factors
    map<string, int> m_map_rebin_factors;

    // Check Hist, if hist not exist in the input file, get warning!
    void CheckHist(TDirectory *dir_input);
    // Get Combined Hist
    TH1F* GetCombinedHist(TH1F *h0, TH1F *h1, TH1F *h20);

public:
    // Constructor
    // default
    ggFKlambdaCombinedTool();
    // deconstructor
    - ggFKlambdaCombinedTool();
    // construct really use
    ggFKlambdaCombinedTool(const string& input_file, const string& output_file);

    // Get Methods

    // Set Methods


    // Combine and write to output file
    void CombineAndWrite();

    // Useful toolkit, transform such 1.0 to "1p0", -1.0 to "n1p0"
    std::string transformFloat(float KLambda);
}