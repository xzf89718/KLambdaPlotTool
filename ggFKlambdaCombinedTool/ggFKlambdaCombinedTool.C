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
    delete m_ delete input_file;
    delete output_file;
    delete m_h0;
    delete m_h1;
    delete m_h20;
    delete m_combined;

    cout << "ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool: Deleted" << endl;
}

ggFKlambdaCombinedTool::ggFKlambdaCombinedTool(string input_file, string output_file)
{
    // For input file, read only!
    m_input_file = TFile::Open(input_file.c_str(), "READ");
    // For output file, we should update it!
    m_output_file = TFile::Open(output_file.c_str(), "UPDATE");
    if (!m_input_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: input_file not found, exit!" std::abort();
    }
    if (!m_output_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: could not open output_file, exit!" std::abort();
    }

    m_h0 = nullptr;
    m_h1 = nullptr;
    m_h20 = nullptr;
    m_combined = nullptr;
    m_vec_HistNameManagers = vector<HistNameManager>();

    cout << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: Initialize with input and output file" << endl;
}

void ggFKlambdaCombinedTool::CombineAndWrite(float klambda)
{

    // For more details about the method here, check Alessandra Betti's talk
    // Methods to obtain signal templates for HH signals with couplings variations
    // We take KT = 1 here
    auto h_cup = new TH1F(*m_h0);
    
    // now consider kappa_t = 1, kappa_lambda
    // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
    auto Kappa_t = 1.0;
    // now switch to lambda = 20.0
    auto d_lambda = 20.0;
    auto k1 = Kappa_t * Kappa_t *
              ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) +
               (d_lambda - 1) * pow(KLambda, 2) - (pow(d_lambda, 2) - 1.) * Kappa_t * KLambda) /
              (pow(d_lambda, 2) - d_lambda);
    auto k2 = Kappa_t * Kappa_t *
              (pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * pow(KLambda, 2)) / (pow(d_lambda, 2) - d_lambda);
    auto k3 = Kappa_t * Kappa_t *
              (pow(KLambda, 2) - Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
    h_cup->Sumw2();
    h_cup->Add(m_h0, m_h1,
               k1,
               k2);
    m_combined = new TH1F(*h_cup);
    m_combined->Sumw2();
    m_combined->Add(m_combined, m_h20,
                    1,
                    k3);

    m_combined->SetName(hist_name_reweighted.c_str());
    m_combined->SetTitle(hist_name_reweighted.c_str());
    m_combined->Write();
    delete m_combined;
    delete h_cup;
}

bool ggFKlambdaCombinedTool::GetHistAndCheck(string h0_name, string h1_name, string h20_name)
{
    m_h0 = (TH1F *)input_dir->Get(h0_name.c_str());
    m_h1 = (TH1F *)input_dir->Get(h1_name.c_str());
    m_h20 = (TH1F *)input_dir->Get(h20_name.c_str());

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
        m_h0 = nullptr;
        m_h1 = nullptr;
        m_h20 = nullptr;
        return false;
    }
    else
    {
        m_h0 = (TH1F *)input_dir->Get(h0_name.c_str())->Clone();
        m_h1 = (TH1F *)input_dir->Get(h1_name.c_str())->Clone();
        m_h20 = (TH1F *)input_dir->Get(h20_name.c_str())->Clone();
        return true;
    }
}

void ggFKlambdaCombinedTool::LoopOverHistNameManagers()
{   
    if (m_vec_HistNameManagers.size() == 0)
    {
        clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: ERROR m_vec_HistNameManagers is empty, try SetDefaultHistNameManager first if you want default setup!"<< endl;
        std::abort();
    }

    // Iter all HistNameManager in m_vec_HistNameManagers
    for (auto iter_hnm = m_vec_HistNameManagers.begin(); iter_hnm != m_vec_HistNameManagers.begin(); iter_hnm++)
    {

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

void SetDefaultHistNameManager()
{
    // Setup Preselection
    HistNameManager hnm_Preselection();
    hnm_Preselection.SetDefaultBaseName();
    hnm_Preselection.SetDefaultRegions();
    hnm_Preselection.SetDefaultVariables();
    hnm_Preselection.SetDirName("Preselection");

    m_vec_HistNameManagers.push_back(hnm_Preselection);

    // Setup BDTScorePreselection
    HistNameManager hnm_BDTScorePreselection();
    hnm_BDTScorePreselection.SetDefaultBaseName();
    hnm_BDTScorePreselection.SetDefaultRegions();
    hnm_BDTScorePreselection.hnm_BDTScorePreselection.SetDirName("BDTScorePreselection");

    m_vec_HistNameManagers.push_back(hnm_BDTScorePreselection);

    // Setup PNNScorePreselection
    HistNameManager hnm_PNNScorePreselection();
    hnm_PNNScorePreselection.SetDefaultBaseName();
    hnm_PNNScorePreselection.SetDefaultRegions();
    hnm_PNNScorePreselection.SetDefaultPNNVariables();
    hnm_PNNScorePreselection.SetDirName("PNNScorePreselection");

    m_vec_HistNameManagers.push_back(hnm_PNNScorePreselection);
}