/// \file
/// \ingroup tutorial_hist
/// \notebook
/// Example of a fit residual plot.
///
/// Creates a histogram filled with random numbers from a gaussian distribution
/// and fits it with a standard gaussian function. The result is passed to the `TRatioPlot`
/// constructor. Additionally, after calling `TRatioPlot::Draw` the upper and lower y axis
/// titles are modified.
/// Confidence interval bands are automatically drawn on the bottom (but can be disabled by draw option `nobands`.
///
/// \macro_image
/// \macro_code
///
/// \author Paul Gessinger
#include <vector>
#include <string>
#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TH1F.h"


void GetYields() 
{

        std::string Dir_name = {"Preselection"};
        auto KLReweight_py8 = new TFile("./output/KLReweight_py8.root", "READ");
        auto KLReweight_herwig7 = new TFile("./output/KLReweight.aroot", "READ");
        //std::string h1_name = {"hhttbbKL10p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"}; 
        //std::string h2_name = {"hhttbbKL5p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"};
        //std::string figure_name = {"./plots/test.pdf"};

        auto dir = (TDirectory*)KLReweight_py8->Get(Dir_name.c_str());
        
        ofstream fout;
        fout.open("./plots/Yields.txt", ios::app);
        if(!fout)
        {
                cout<<"Cant open output file!"<<endl;
        }
        else
        {       for(auto &&hist_name: *(dir->GetListOfKeys()))
                {
                    auto h1 = (TH1F*)dir->Get(hist_name->GetTitle());
                    fout <<"PY8 Yields: "<<hist_name->GetTitle() <<" "<< h1->Integral() <<endl;
        }       }
        KLReweight_py8->Close();
        KLReweight_herwig7->Close();
        
}
