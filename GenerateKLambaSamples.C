// This macro should be used as 
// root -b -q -q THISMACRO.C only!

// Author: Zifeng, Xu
// Usage: Generate differente signal templates using reco-level histograms
// Search HERE_IS_BASE_FILE, modify it to your own generated .root file
// which contains KL=0, 1, 20 samples!
#include <vector>
#include <iostream>
#include <string>
#include <map>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TList.h"




// 
void GenerateKLambaSamples(const double KLambda)
{   
        using std::cout;
        using std::endl;
        using std::cerr;
        using std::clog;

        // A kindly remind here: dure to the limit of std:to_string, this program cant handle 
        // input more than 5 digit decimal
        // Transform input KLambda to string form, such as 1p00000
        std::string string_KLambda = std::to_string(KLambda);
        auto index_of_point = string_KLambda.find(".");
        string_KLambda.replace(index_of_point, index_of_point, "p");

        vector<std::string> variable_names;
        vector<std::string> region_names;
        //vector<std::string> base_names;
        map<std::string, std::string> base_names;

        //Add all variables to variable_names
        variable_names.push_back("mBB");
        variable_names.push_back("mMMC");
        variable_names.push_back("mHH");
        variable_names.push_back("mHHScaled");
        variable_names.push_back("dRBB");
        variable_names.push_back("pTBB");
        variable_names.push_back("dRTauTau");
        variable_names.push_back("pTTauTau");
        variable_names.push_back("dPhiBBTauTau");
        variable_names.push_back("dRBBTauTau");
        variable_names.push_back("pTBalance");
        variable_names.push_back("MET");
        variable_names.push_back("nJets");
        variable_names.push_back("Yield");
        variable_names.push_back("metSig");
        variable_names.push_back("metSigPU");
        variable_names.push_back("Jet0Pt");
        variable_names.push_back("Jet1Pt");
        variable_names.push_back("pTB0");
        variable_names.push_back("pTB1");
        variable_names.push_back("LeadJetPt");
        variable_names.push_back("SubleadJetPt");
        variable_names.push_back("LeadJetEta");
        variable_names.push_back("SubleadJetEta");
        variable_names.push_back("Jet0Eta");
        variable_names.push_back("Jet1Eta");
        variable_names.push_back("Tau0Pt");
        variable_names.push_back("Tau1Pt");
        variable_names.push_back("Tau0Eta");
        variable_names.push_back("Tau1Eta");
        variable_names.push_back("Tau0Ntrk");
        variable_names.push_back("Tau1Ntrk");
        variable_names.push_back("dPhiTauTau");

        // Add region names
        region_names.push_back("2tag2pjet_0ptv_LL_OS");

        // Add base_sample name
        // Here we use KL 1p0 10p0 generated and KL 20p0 reweighted from 10p0 
        // (now maybe we can switch to reweigted from 1p0 in the next step!)
        base_names.insert(std::pair<std::string, std::string>("base", "hhttbbKL"));
        base_names.insert(std::pair<std::string, std::string>("1p0", "hhttbbKL1p0"));
        base_names.insert(std::pair<std::string, std::string>("10p0", "hhttbbKL10p0"));
        // base_names.push_back("hhttbbKL20p0from1p0");
        base_names.insert(std::pair<std::string, std::string>("20p0", "hhttbbKL20p0from10p0"));



        // HERE_IS_BASE_FILE
        auto base_file = new TFile("./output/KLReweight_py8.root", "READ");
        auto output_file = new TFile("./output/KLRecoReweigt_py8.root", "UPDATE");
        // auto output_file = new TFile("./output/KLRecoReweigt.root", "UPDATE");

        // auto base_file = new TFile("./output/KLReweight.root", "READ");
        if (!base_file)
        {
                cerr << "error: " << "Can't open basefile, Check HERE_IS_BASE!" << endl;
                return;
        }
        // Now only consider Preselection
        // vector<std::string> Dir_names;
        std::string dirName("Preselection");
        auto dir_Preselection = (TDirectory*)base_file->Get(dirName.c_str());
        if (!dir_Preselection)
        {
                cerr << "error: " << "Can't open TDirectory, Check dirName!" << endl;
                return;
        }

        // Create Preselection directory in outputfile and write histogram
        if (!outputfile->Get(directory))
        {
                outputfile->mkdir(dirName);
                clog << "Warning: Cant find Preselection in output file, mkdir!" << endl;
        }
        //
        
        outputfile->cd(dirName);
        for (auto iter_variable = variable_name.begin(); iter_variable != variable_name.end(); iter_variable ++)
        {       
                for(auto iter_region = region_names.begin(); iter_region != region_names.end(); iter_region ++)
                {
                        // Reweighted histogram name
                        auto hist_name_reweighted = base_names.at("base") + string_KLambda + "fromReco" + \
                                                    "_" + *iter_region + "_" + *iter_variable;
                        // Get three base histogram
                        auto h0_name = base_names.at("0p0") + "_" + *iter_region + "_" + *iter_variable;
                        auto h1_name = base_names.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                        auto h10_name = base_names.at("10p0") + "_" + *iter_region + "_" + *iter_variable;

                        auto h0 = (TH1F*)dir_Preselection->Get(h0_name);
                        auto h1 = (TH1F*)dir_Preselection->Get(h1_name);
                        auto h10 = (TH1F*)dir_Preselection->Get(h10_name);

                        if(!h0)
                        {
                                clog << "Warning: Cant find base histogram h0" << endl;
                        }
                        if(!h1)
                        {
                                clog << "Warning: Cant find base histogram h1" << endl;
                        }
                        if(!h10)
                        {
                                clog << "Warning: Cant find base histogram h10" << endl;
                        }
                        if(!h0 || !h1 || !h10)
                        {        
                                continue;
                        }
                        // Add h0, h1, h10 follow the fomula
                        //

                }
        }

        return;
}










