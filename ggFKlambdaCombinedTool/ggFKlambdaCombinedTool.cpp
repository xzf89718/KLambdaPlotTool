#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>

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

#include "ggFKlambdaCombinedTool.hh"
#include "HistNameManager.hh"

using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::pow;

ggFKlambdaCombinedTool::ggFKlambdaCombinedTool()
{
    clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: WARNING: default initialize, you may meet some problems! " << endl;
}

ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool()
{
    // delete m_input_dir;
    // delete m_output_dir;
    m_input_file->Close();
    m_output_file->Close();
    delete m_input_file;
    delete m_output_file;

    // delete m_h0;
    // delete m_h1;
    // delete m_h20;
    // delete m_combined;

    for (auto iter_p = m_vec_HistNameManagers.begin(); iter_p != m_vec_HistNameManagers.end(); iter_p++)
    {
        cout << "ggFKlambdaCombinedTool:: ~ggFKlambdaCombinedTool: INFO delete HistNameManager" << endl;
        delete *iter_p;
    }
    cout << "ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool: Deleted" << endl;
}

ggFKlambdaCombinedTool::ggFKlambdaCombinedTool(const string &input_file, const string &output_file)
{
    // For input file, read only!
    m_input_file = TFile::Open(input_file.c_str(), "READ");
    // For output file, we should update it!
    m_output_file = TFile::Open(output_file.c_str(), "UPDATE");
    if (!m_input_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: input_file not found, exit!" << endl;
        std::abort();
    }
    if (!m_output_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: could not open output_file, exit!" << endl;
        std::abort();
    }

    m_input_dir = nullptr;
    m_output_dir = nullptr;
    m_h0 = nullptr;
    m_h1 = nullptr;
    m_h20 = nullptr;
    m_combined = nullptr;
    m_vec_HistNameManagers = vector<HistNameManager *>();
    m_reweighted_hist_name = string();
    m_debug = false;

    cout << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: Initialize with input and output file" << endl;
}

void ggFKlambdaCombinedTool::CombineOneHist(float klambda)
{

    // For more details about the method here, check Alessandra Betti's talk
    // Methods to obtain signal templates for HH signals with couplings variations
    // We take KT = 1 here
    auto h_cup = TH1F(*m_h0);

    // now consider kappa_t = 1, kappa_lambda
    // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
    auto Kappa_t = 1.0;
    // now switch to lambda = 20.0
    auto d_lambda = 20.0;
    auto k1 = Kappa_t * Kappa_t *
              ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) +
               (d_lambda - 1) * pow(klambda, 2) - (pow(d_lambda, 2) - 1.) * Kappa_t * klambda) /
              (pow(d_lambda, 2) - d_lambda);
    auto k2 = Kappa_t * Kappa_t *
              (pow(d_lambda, 2) * Kappa_t * klambda - d_lambda * pow(klambda, 2)) / (pow(d_lambda, 2) - d_lambda);
    auto k3 = Kappa_t * Kappa_t *
              (pow(klambda, 2) - Kappa_t * klambda) / (pow(d_lambda, 2) - d_lambda);
    h_cup.Sumw2();
    h_cup.Add(m_h0, m_h1,
              k1,
              k2);
    m_combined = new TH1F(h_cup);
    m_combined->Sumw2();
    m_combined->Add(m_combined, m_h20,
                    1,
                    k3);

    // delete m_h0;
    // delete m_h1;
    // delete m_h20;
}
void ggFKlambdaCombinedTool::DeleteHists()
{
    delete m_h0;
    delete m_h1;
    delete m_h20;
    delete m_combined;
}

// void ggFKlambdaCombinedTool::DrawDebugPlots()
// {

//     gROOT->SetStyle("ATLAS");
//     gROOT->SetStyle(0);

//     std::ofstream debug_k;
//     debug_k.open("./output/All_k.txt", std::ios::app);
//     debug_k << "KLambda: " << KLambda << " k1: " << k1 << " k2: " << k2 << " k3: " << k3 << endl;
//     debug_k.close();

//     auto c1 = new TCanvas("c1", "c1", 800 * 1.5, 600 * 1.5);
//     gStyle->SetOptStat(0);
//     m_combined->Draw("HIST");
//     m_h1->Draw("HIST SAME");
//     m_h20->Draw("HIST SAME");
//     m_h0->Draw("HIST SAME");
//     m_h0->SetLineColor(kRed);
//     m_h1->SetLineColor(kGreen);
//     m_h20->SetLineColor(kRed + 2);
//     m_combined->SetLineColor(kGreen + 2);
//     c1->BuildLegend();
//     // c1->SetLogy();
//     c1->SaveAs(("./output/plots/" + m_reweighted_hist_name + ".png").c_str());
//     //cout << "hist_name_reweighted: " << hist_name_reweighted << endl;
//     delete c1;

//     // here i want to show the error propagation!
//     auto c2 = new TCanvas("c2", "c2", 800 * 1.5, 600 * 1.5);
//     // Get reweighted from basefile (truth-level)
//     auto hist_name_truth = base_names.at("base") + string_KLambda + "from1p0" + "_" + *iter_region + "_" + *iter_variable;
//     //cout << "DEBUG: " << hist_name_truth << endl;
//     // create 3 TGraph to store error of 3 basis!
//     auto NBinX = (Int_t)h0->GetNbinsX();
//     auto error_0 = new float[NBinX];
//     auto error_1 = new float[NBinX];
//     auto error_20 = new float[NBinX];
//     auto h_truth = (TH1F *)dir_input->Get(hist_name_truth.c_str());
//     auto error_truth = new float[NBinX];
//     // validate error propagate in truth-reweight level!
//     auto bincontent_1 = new float[NBinX];
//     auto bincontent_truth = new float[NBinX];
//     if (!h_truth)
//     {
//         clog << "Cant find h_truth, no h_truth has been added!" << endl;
//         delete h_truth;
//     }
//     else
//     {
//         delete h_truth;
//         h_truth = (TH1F *)dir_input->Get(hist_name_truth.c_str())->Clone();
//     }

//     // create 1 TGraph to store error of combined one!
//     auto error_reweight = new float[NBinX];
//     auto error_calculated = new float[NBinX];

//     // XAxis for TGraph
//     auto x_for_tgraph = new float[NBinX];
//     for (auto i = 0; i < NBinX; i++)
//     {
//         error_0[i] = h0->GetBinError(i);
//         error_1[i] = h1->GetBinError(i);
//         bincontent_1[i] = h1->GetBinContent(i);
//         error_20[i] = h20->GetBinError(i);
//         if (h_truth)
//         {
//             auto content_truth = h_truth->GetBinContent(i);
//             if (content_truth > 0.0)
//             {
//                 error_truth[i] = (h_truth->GetBinError(i)) / (content_truth);
//             }
//             else
//             {
//                 error_truth[i] = -1;
//             }
//             bincontent_truth[i] = h_truth->GetBinContent(i);
//         }
//         // Get error of reweight one
//         auto content_cup_2 = h_cup_2->GetBinContent(i);
//         if (content_cup_2 > 0.0)
//         {
//             error_reweight[i] = (h_cup_2->GetBinError(i)) / content_cup_2;
//         }
//         else
//         {
//             error_reweight[i] = -1;
//         }
//         // try to recalculate!
//         error_calculated[i] = std::sqrt(pow(error_0[i] * k1, 2) + pow(error_1[i] * k2, 2) + pow(error_20[i] * k3, 2));
//         x_for_tgraph[i] = h0->GetBinCenter(i);

//         //cout << "cal error: " <<error_calculated[i]<< " reweighted error: "<< error_reweight[i] \
//                                 << " KL=1 error: " << error_1[i] << " propagate: " << bincontent_truth[i] / bincontent_1[i] \
//                                     <<" is it fit my assumption?" << (bincontent_truth[i]/bincontent_truth[i] * error_1[i] - error_truth[i]  < 0.00001) << endl;
//     }
//     // Initialize TGraph for them
//     auto g0 = new TGraph(NBinX, x_for_tgraph, error_0);
//     g0->SetMarkerStyle(10);
//     g0->SetName("g0");
//     auto g1 = new TGraph(NBinX, x_for_tgraph, error_1);
//     g1->SetMarkerStyle(12);
//     g1->SetName("g1");
//     auto g20 = new TGraph(NBinX, x_for_tgraph, error_20);
//     g20->SetMarkerStyle(14);
//     g20->SetName("g20");
//     auto gtruth = new TGraph(NBinX, x_for_tgraph, error_truth);
//     gtruth->SetMarkerStyle(21);
//     gtruth->SetMarkerColor(2);
//     gtruth->SetName("reweight_from_truth");
//     gtruth->SetTitle("reweigth_from_truth");
//     gtruth->SetDrawOption("AP");
//     auto greweight = new TGraph(NBinX, x_for_tgraph, error_reweight);
//     greweight->SetMarkerStyle(22);
//     greweight->SetMarkerColor(4);
//     greweight->SetName("combine_at_reco-level");
//     greweight->SetTitle("combine_at_reco-level");
//     greweight->SetDrawOption("AP");
//     auto gcalculated = new TGraph(NBinX, x_for_tgraph, error_calculated);
//     gcalculated->SetMarkerStyle(23);
//     gcalculated->SetMarkerColor(6);
//     gcalculated->SetName("gcalculated");
//     gcalculated->SetTitle("gcalculated");
//     gcalculated->SetDrawOption("AP");
//     //g0->Draw();
//     //g1->Draw("SAME");
//     //g20->Draw("SAME");
//     // Let's try multigraph
//     //
//     auto mg = new TMultiGraph("mg", "Shape_only");
//     if (h_truth)
//     {
//         mg->Add(gtruth);
//         mg->Add(greweight);
//         //mg->Add(greweight);
//         //mg->Add(gcalculated);
//     }
//     else
//     {
//         //mg->Add(gtruth);
//         //mg->Add(gcalculated);
//     }
//     mg->Draw("AP");
//     gPad->Update();
//     gPad->BuildLegend();
//     mg->GetXaxis()->SetTitle((*iter_variable).c_str());
//     mg->GetYaxis()->SetTitle("Error");
//     gPad->Update();
//     c2->SaveAs(("./output/debug_plots/" + hist_name_reweighted + ".png").c_str());

//     delete g0;
//     delete g1;
//     delete g20;
//     delete gtruth;
//     delete greweight;
//     delete gcalculated;
//     delete mg;

//     delete[] x_for_tgraph;
//     delete[] error_0;
//     delete[] error_1;
//     delete[] error_20;
//     delete[] error_truth;
//     delete[] error_reweight;
//     delete[] error_calculated;
//     delete[] bincontent_truth;
//     delete[] bincontent_1;
//     delete c2;
// }
bool ggFKlambdaCombinedTool::GetHistAndCheck(const string &h0_name, const string &h1_name, const string &h20_name)
{

    m_h0 = (TH1F *)m_input_dir->Get(h0_name.c_str());
    m_h1 = (TH1F *)m_input_dir->Get(h1_name.c_str());
    m_h20 = (TH1F *)m_input_dir->Get(h20_name.c_str());

    if (!m_h0)
    {
        clog << "Warning: Cant find base histogram h0" << endl;
    }
    if (!m_h1)
    {
        clog << "Warning: Cant find base histogram h1" << endl;
    }
    if (!m_h20)
    {
        clog << "Warning: Cant find base histogram h20" << endl;
    }
    if (!m_h0 || !m_h1 || !m_h20)
    {
        clog << "ggFKlambdaCombinedTool::GetHistAndCheck: This histogram is not exist in the base_file, skip" << endl;
        delete m_h0;
        delete m_h1;
        delete m_h20;
        return false;
    }
    else
    {
        m_h0 = (TH1F *)m_input_dir->Get(h0_name.c_str())->Clone();
        m_h1 = (TH1F *)m_input_dir->Get(h1_name.c_str())->Clone();
        m_h20 = (TH1F *)m_input_dir->Get(h20_name.c_str())->Clone();
        return true;
    }
}

void ggFKlambdaCombinedTool::LoopOverHistNameManagers(float klambda)
{
    if (m_vec_HistNameManagers.size() == 0)
    {
        clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: ERROR m_vec_HistNameManagers is empty, try SetDefaultHistNameManager first if you want default setup!" << endl;
        std::abort();
    }
    else
    {
        cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: INFO There are"
             << " " << m_vec_HistNameManagers.size() << " HistNameManager" << endl;
    }

    // Get m_input_dir and m_output_dir

    // Iter all HistNameManager in m_vec_HistNameManagers
    for (auto iter_hnm = m_vec_HistNameManagers.begin(); iter_hnm != m_vec_HistNameManagers.end(); iter_hnm++)
    {
        // Get all containers HistNameManager hold
        auto vec_regions = (*iter_hnm)->GetRegions();
        auto vec_variables = (*iter_hnm)->GetVariables();
        auto map_basenames = (*iter_hnm)->GetBaseMap();
        auto str_dirname = (*iter_hnm)->GetDirName();

        // Check or Create the diretory!
        m_input_dir = (TDirectory *)m_input_file->Get(str_dirname.c_str());
        if (!m_input_dir)
        {
            clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: ERROR: Input directory not exist, skip and check the HistNameManager DirName! Skip." << endl;
            continue;
        }

        m_output_dir = (TDirectory *)m_output_file->Get(str_dirname.c_str());
        if (!m_output_dir)
        {
            clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: WARNING: Output directory not exist in output file, now try to mkdir!" << endl;
            m_output_file->mkdir(str_dirname.c_str());
        }
        else
        {
            cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: Now switch to output directory " << str_dirname << endl;
        }
        m_output_file->cd(str_dirname.c_str());

        // Loop over these containers!
        for (auto iter_region = vec_regions.begin(); iter_region != vec_regions.end(); iter_region++)
        {

            for (auto iter_variable = vec_variables.begin(); iter_variable != vec_variables.end(); iter_variable++)
            {
                auto hist_name_reweighted = map_basenames.at("base") + TransformFloat(klambda) + "fromReco" +
                                            "_" + *iter_region + "_" + *iter_variable;
                m_reweighted_hist_name = hist_name_reweighted;
                // Get three base histogram
                auto h0_name = map_basenames.at("0p0") + "_" + *iter_region + "_" + *iter_variable;
                auto h1_name = map_basenames.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                auto h20_name = map_basenames.at("20p0") + "_" + *iter_region + "_" + *iter_variable;

                // GetHistAndCheck
                if (GetHistAndCheck(h0_name, h1_name, h20_name))
                {
                    // CombineOneHist
                    CombineOneHist(klambda);

                    // Write histogram to ouput file!
                    m_combined->SetName(hist_name_reweighted.c_str());
                    m_combined->SetTitle(hist_name_reweighted.c_str());
                    m_combined->Write();

                    // cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: Write one histogram into output file" << endl;
                    // Delete histograms 
                    DeleteHists();
                }
                else
                {
                    cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: WARNING: Base histograms not exist! Skipped." << endl;
                    cout << h0_name << "\n"
                         << h1_name << "\n"
                         << h20_name << endl;
                }
            }
        }

        // Go back to top of the TDirectory
        m_output_file->cd("../");
        delete m_input_dir;
        delete m_output_dir;
    }
}

std::string ggFKlambdaCombinedTool::TransformFloat(float KLambda)
{
    // input: any float KLambda
    // output: somethin like (string) 1p0

    // A kindly remind here: dure to the limit of std:to_string, this program cant handle
    // input more than 5 digit decimal
    // Transform input KLambda to string form, such as 1p00000
    std::string string_KLambda = std::to_string(KLambda);
    auto index_of_point = string_KLambda.find(".");
    if (index_of_point != -1)
        string_KLambda.replace(index_of_point, 1, "p");
    //for now, we only need 2 digit decimal
    //string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 4);
    // now, we noly need 1 digit decimal
    string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 5);
    //if KLambda<0, we need to substitute the "-" with "n"
    auto index_of_minus = string_KLambda.find("-");
    if (index_of_minus != -1)
        string_KLambda.replace(index_of_minus, 1, "n");

    return string_KLambda;
}
void ggFKlambdaCombinedTool::SetDebug(bool debug_or_not)
{
    m_debug = debug_or_not;
    cout << "ggFKlambdaCombinedTool::SetDebug: INFO Now set debug " << debug_or_not << endl;
}
void ggFKlambdaCombinedTool::SetDefaultHistNameManager()
{
    // Setup Preselection
    auto hnm_Preselection = new HistNameManager;
    hnm_Preselection->SetDefaultBaseName();
    hnm_Preselection->SetDefaultRegions();
    hnm_Preselection->SetDefaultVariables();
    hnm_Preselection->SetDirName("Preselection");

    AddHistNameManager(hnm_Preselection);

    // Setup PNNScorePreselection
    auto hnm_PNNScorePreselection = new HistNameManager;
    hnm_PNNScorePreselection->SetDefaultBaseName();
    hnm_PNNScorePreselection->SetDefaultRegions();
    hnm_PNNScorePreselection->SetDefaultPNNVariables();
    hnm_PNNScorePreselection->SetDirName("PNNScorePreselection");

    AddHistNameManager(hnm_PNNScorePreselection);

    // Setup NN
    auto hnm_NNScorePreselection = new HistNameManager;
    hnm_NNScorePreselection->SetDefaultBaseName();
    hnm_NNScorePreselection->SetDefaultRegions();
    hnm_NNScorePreselection->SetDefaultNNVariables();
    hnm_NNScorePreselection->SetDirName("NNScorePreselection");

    AddHistNameManager(hnm_NNScorePreselection);

    // Setup SMBDT
    auto hnm_BDTScorePreselection = new HistNameManager;
    hnm_BDTScorePreselection->SetDefaultBaseName();
    hnm_BDTScorePreselection->SetDefaultRegions();
    hnm_BDTScorePreselection->SetDefaultBDTVariables();
    hnm_BDTScorePreselection->SetDirName("BDTScorePreselection");

    AddHistNameManager(hnm_BDTScorePreselection);
}
