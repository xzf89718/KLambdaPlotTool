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
#include <iostream>
#include <fstream>
#include <algorithm>

#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TH1F.h"


void GetYields() 
{
    using std::cout;
    using std::endl;
    std::string Dir_name = {"Preselection"};
    // auto KLReweight_py8 = new TFile("./output/KLReweight_py8.root", "READ");
    auto KLReweight_py8 = new TFile("./output/KLReweight_py8_moreKLSignals.root", "READ");
    //auto KLReweight_herwig7 = new TFile("./output/KLReweight.root", "READ");
    auto KLReco_py8 = new TFile("./output/KLRecoReweigt_py8.root","READ");
    //std::string h1_name = {"hhttbbKL10p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"}; 
    //std::string h2_name = {"hhttbbKL5p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"};
    //std::string figure_name = {"./plots/test.pdf"};

    auto dir = (TDirectory*)KLReweight_py8->Get(Dir_name.c_str());
    auto dir_2 = (TDirectory*)KLReco_py8->Get(Dir_name.c_str());
    std::ofstream fout;
    std::ofstream fout2;
    fout.open("./plots/Yields.txt");
    fout2.open("./plots/Yields_Reco.txt");
    if(!fout && !fout2)
    {
        cout<<"Cant open output file!"<<endl;
    }
    else
    {       
        for(auto &&hist_name: *(dir->GetListOfKeys()))
        {
            auto h1 = (TH1F*)dir->Get(hist_name->GetTitle());
            std::string full_name(hist_name->GetTitle());
            auto is_it_SR = full_name.find("2tag2pjet_0ptv_LL_OS");
            if (is_it_SR !=std::string::npos){
                fout <<"PY8 Yields: "<<hist_name->GetTitle() <<" "<< h1->Integral() <<endl;
                cout << "This hist in signal region!" << endl;
            }
            else
            {
                cout << "Not in Signal Region, dont write it in to Yields.txt" << endl;
            }
            delete h1;
        }
        for(auto &&hist_name: *(dir_2->GetListOfKeys()))
        {
            auto h1 = (TH1F*)dir_2->Get(hist_name->GetTitle());
            if(h1)
                fout2 <<"Reco Yields: "<<hist_name->GetTitle() <<" "<< h1->Integral() <<endl;
            delete h1;
        }
    }

    fout.close();
    fout2.close();
    KLReweight_py8->Close();
    // KLReweight_herwig7->Close();
    KLReco_py8->Close();
}
