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
#define DEBUG_KLREWEIGT 1
void GenerateKLambaSamples(const double KLambda)
{   
        using std::cout;
        using std::endl;
        using std::cerr;
        using std::clog;
        using std::pow;
        // Base not reweight!
        if (KLambda == 1.0 || KLambda == 10.0 || KLambda == 20.0)
        {
                clog << "KL equals to one of the base, skip KLambda = " << KLambda << endl;
                return;
        }

        // A kindly remind here: dure to the limit of std:to_string, this program cant handle 
        // input more than 5 digit decimal
        // Transform input KLambda to string form, such as 1p00000
        std::string string_KLambda = std::to_string(KLambda);
        auto index_of_point = string_KLambda.find(".");
        if(index_of_point != -1)
                string_KLambda.replace(index_of_point, 1, "p");
        //for now, we only need 2 digit decimal
        //string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 4);
        // now, we noly need 1 digit decimal
        string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 4);
        //if KLambda<0, we need to substitute the "-" with "n"
        auto index_of_minus = string_KLambda.find("-");
        if (index_of_minus != -1)
                string_KLambda.replace(index_of_minus, 1, "n");

        // STL containeers for variable, region and base
        vector<std::string> variable_names;
        vector<std::string> region_names;
        //vector<std::string> base_names;
        std::map<std::string, std::string> base_names;

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
        if (!base_file)
        {
                cerr << "error: " << "Can't open basefile, Check HERE_IS_BASE!" << endl;
                return;
        }
        auto output_file = new TFile("./output/KLRecoReweigt_py8.root", "UPDATE");
        if (!output_file)
        {
                cerr << "error: " << "Can't open outputfile, Check HERE_IS_BASE!" << endl;
                return;
        }
        // write base infomation into the output_file
        double base1 = 1;
        double base2 = 10;
        double base3 = 20;
        auto base_info = new TTree("base_info", "base_info");
        base_info->Branch("base1", &base1, "base1/D");
        base_info->Branch("base2", &base2, "base2/D");
        base_info->Branch("base3", &base3, "base3/D");
        base_info->Fill();
        output_file->Write("base_info");

        // Now only consider Preselection
        // vector<std::string> Dir_names;
        std::string dirName("Preselection");
        auto dir_Preselection = (TDirectory*)base_file->Get(dirName.c_str());
        if (!dir_Preselection)
        {
                cerr << "error: " << "Can't open TDirectory, Check dirName!" << endl;
                return;
        }

        // Create Preselection directory in output_file and write histogram
        if (!output_file->Get(dirName.c_str()))
        {
                output_file->mkdir(dirName.c_str());
                clog << "Warning: Cant find Preselection in output file, mkdir!" << endl;
        }
        //
        output_file->cd(dirName.c_str());

        // start to combine the histogram and writte ! 
        for (auto iter_variable = variable_names.begin(); iter_variable != variable_names.end(); iter_variable ++)
        {       
                for(auto iter_region = region_names.begin(); iter_region != region_names.end(); iter_region ++)
                {
                        // Reweighted histogram name
                        auto hist_name_reweighted = base_names.at("base") + string_KLambda + "fromReco" + \
                                                    "_" + *iter_region + "_" + *iter_variable;
                        // Get three base histogram
                        auto h1_name = base_names.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                        auto h10_name = base_names.at("10p0") + "_" + *iter_region + "_" + *iter_variable;
                        auto h20_name = base_names.at("20p0") + "_" + *iter_region + "_" + *iter_variable;

                        auto h1 = (TH1F*)dir_Preselection->Get(h1_name.c_str());
                        auto h10 = (TH1F*)dir_Preselection->Get(h10_name.c_str());
                        auto h20 = (TH1F*)dir_Preselection->Get(h20_name.c_str());

                        if(!h1)
                        {
                                clog << "Warning: Cant find base histogram h0" << endl;
                        }
                        if(!h10)
                        {
                                clog << "Warning: Cant find base histogram h1" << endl;
                        }
                        if(!h20)
                        {
                                clog << "Warning: Cant find base histogram h10" << endl;
                        }
                        if(!h1 || !h10 || !h20)
                        {        
                                clog << "This histogram is not exist in the base_file, skip" << endl;
                                continue;
                        }
                        // Add h1, h10, h20 follow the fomula
                        //
                        delete h1;
                        delete h10;
                        delete h20;

                        h1 = (TH1F*)dir_Preselection->Get(h1_name.c_str())->Clone();
                        h10 = (TH1F*)dir_Preselection->Get(h10_name.c_str())->Clone();
                        h20 = (TH1F*)dir_Preselection->Get(h20_name.c_str())->Clone();

                        // For more details about the method here, check Alessandra Betti's talk 
                        // Methods to obtain signal templates for HH signals with couplings variations
                        // We take KT = 1 here
                        //
                        //                        auto h_cup = ( 200. / 171. - KLambda * 10. / 57. + pow(KLambda, 2) * 1 / 171) * (*h1)\
                        //                                     + ((-2. / 9.0) + KLambda * 7. / 30. - pow(KLambda, 2) * 1. / 90. )* (*h10) \
                        //                                     + (1. / 19. - KLambda * 11. / 190. + pow(KLambda, 2) * 1. / 190. ) * (*h20);
                        //                        h_cup.SetName(hist_name_reweighted.c_str());
                        auto h_cup = new TH1F(*h1);
                        // here are the 3 coefficient
                        double k1 = 200. / 171. - KLambda * 10. / 57. + pow(KLambda, 2) * 1 / 171;
                        double k2 = (-2. / 9.0) + KLambda * 7. / 30. - pow(KLambda, 2) * 1. / 90.;
                        double k3 = 1. / 19. - KLambda * 11. / 190. + pow(KLambda, 2) * 1. / 190.;
                        h_cup->Sumw2();
                        h_cup->Add(h1, h10, \
                                        k1,\
                                        k2);
                        auto h_cup_2 = new TH1F(*h_cup);
                        h_cup_2->Sumw2();
                        h_cup_2->Add(h_cup_2, h20,\
                                        1,
                                        k3);
                        h_cup_2->SetName(hist_name_reweighted.c_str());
                        h_cup_2->SetTitle(hist_name_reweighted.c_str());
#ifdef DEBUG_KLREWEIGT
                        std::ofstream debug_k;
                        debug_k.open("./output/All_k.txt",std::ios::app);
                        debug_k << "KLambda: " <<KLambda <<" k1: " << k1<<" k2: " << k2<<" k3: " <<k3 <<endl;
                        debug_k.close();
                        auto c1 = new TCanvas("c1", "c1");

                        h10->Draw("");
                        h1->Draw("SAME");
                        h20->Draw("SAME");
                        h_cup_2->Draw("SAME");
                        h1->SetLineColor(kRed);
                        h10->SetLineColor(kGreen);
                        h20->SetLineColor(kRed + 2);
                        h_cup_2->SetLineColor(kGreen + 2);
                        c1->BuildLegend();
                       // c1->SetLogy();
                        c1->SaveAs(("./output/plots/" + hist_name_reweighted + ".pdf").c_str());
                        cout << "hist_name_reweighted: " << hist_name_reweighted << endl;
                        delete c1;
#endif
                        h_cup_2->Write();
                        delete h1;
                        delete h10;
                        delete h20;
                        delete h_cup_2;
                        delete h_cup;
                }
        }

        return;
}
