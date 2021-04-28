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
#include "TLegend.h"

#include "ComparePlot.hh"
#include "HistNameManager.hh"

using std::clog;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

void ComparePlot::DeleteHistsAndCanvas()
{

    delete m_h1;
    delete m_h2;
    delete m_h3;
    delete m_legend;
    delete m_pad1;
    delete m_pad2;
    delete m_c1;
}

bool ComparePlot::GetHistAndCheck(const string &h1_name, const string &h2_name)
{
    m_h1 = (TH1F *)m_input_directory1->Get(h1_name.c_str());
    m_h2 = (TH1F *)m_input_directory2->Get(h2_name.c_str());

    if (!m_h1)
    {
        clog << "ComparePlot::GetHistAndCheck: WARNING Cant find base histogram h1" << endl;
        clog << h1_name << " not exist" << endl;
    }
    if (!m_h2)
    {
        clog << "ComparePlot::GetHistAndCheck: WARNING Cant find base histogram h2" << endl;
        clog << h2_name << " not exist" << endl;
    }
    if (!m_h1 || !m_h2)
    {
        clog << "ComparePlot::GetHistAndCheck: WARNING Skip" << endl;
        delete m_h1;
        delete m_h2;
        return false;
    }
    else
    {
        m_h1 = (TH1F *)m_input_directory1->Get(h1_name.c_str())->Clone();
        m_h2 = (TH1F *)m_input_directory2->Get(h2_name.c_str())->Clone();
        return true;
    }
}

void ComparePlot::PlotRatio()
{
    // Define two gaussian histograms. Note the X and Y title are defined
    // at booking time using the convention "Hist_title ; X_title ; Y_title"
    // TH1F *h1 = new TH1F("h1", "Two gaussian plots and their ratio;x title; h1 and h2 gaussian histograms", 100, -5, 5);
    // TH1F *h2 = new TH1F("h2", "h2", 100, -5, 5);
    // h1->FillRandom("gaus");
    // h2->FillRandom("gaus");

    // push_back basenames

    // auto h1 = (TH1F *)dir_py8->Get(hist_name.c_str())->Clone();
    //auto h2 = (TH1F*)dir_herwig7->Get(hist_name.c_str())->Clone();
    // auto h2 = (TH1F *)dir_Reco->Get(hist_name_reco.c_str())->Clone();
    //cout<< h1->GetNbinsX() <<endl;
    auto h1 = m_h1;
    auto h2 = m_h2;
    h1->Rebin();
    h2->Rebin();
    // Define the Canvas
    m_c1 = new TCanvas("c", "canvas", 800, 800);
    auto c = m_c1;
    gROOT->SetStyle("ATLAS");
    gStyle->SetOptStat(0);
    // Upper plot will be in pad1
    m_pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    auto pad1 = m_pad1;
    pad1->SetBottomMargin(1); // Upper and lower plot are joined
    // pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    h1->SetStats(0);          // No statistics on upper plot
    h1->Draw("HIST E1");      // Draw h1
    h2->Draw("HIST E1 SAME"); // Draw h2 on top of h1
    // Create Legend for histogram!
    //auto legend = new TLegend(0.7, 0.9, 0.48, 0.9);
    m_legend = new TLegend(0.75, 0.8, 0.95, 0.95);
    auto legend = m_legend;
    gStyle->SetLegendTextSize(0.02);
    legend->AddEntry(h1, "truth_level");
    legend->AddEntry(h2, "combine_at_reco_level");
    legend->Draw();

    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h1->GetYaxis()->SetLabelSize(0.);
    // TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    // axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    // axis->SetLabelSize(15);
    // axis->Draw();

    // lower plot will be in pad
    c->cd(); // Go back to the main canvas before defining pad2
    TPad *m_pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    auto pad2 = m_pad2;
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    //pad2->SetGridx(); // vertical grid
    pad2->SetGridy(); // horizontal grid
    h1->GetXaxis()->SetTitle("");
    h1->GetYaxis()->SetTitle("Events");
    pad2->Draw();
    pad2->cd(); // pad2 becomes the current pad

    // Define the ratio plot
    m_h3 = (TH1F *)h1->Clone("h3");
    auto h3 = m_h3;
    h3->SetLineColor(kBlack);
    h3->SetMinimum(0.8);  // Define Y ..
    h3->SetMaximum(1.35); // .. range
    h3->Sumw2();
    h3->SetStats(0); // No statistics on lower plot
    h3->Divide(h2);
    h3->SetMarkerStyle(21);
    h3->Draw("ep"); // Draw the ratio plot

    // h1 settings
    h1->SetLineColor(kBlue + 1);
    h1->SetLineWidth(2);

    // Y axis h1 plot settings
    h1->GetYaxis()->SetTitleSize(20);
    h1->GetYaxis()->SetTitleFont(43);
    h1->GetYaxis()->SetTitleOffset(1.55);

    // h2 settings
    h2->SetLineColor(kRed);
    h2->SetLineWidth(2);

    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title

    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("ratio h1/h2 ");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(20);
    h3->GetYaxis()->SetTitleFont(43);
    h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(15);

    // X axis ratio plot settings
    // if (*iter_variable == "mHH" || *iter_variable == "mMMC" || *iter_variable == "mHH")
    // {
    //     h3->GetXaxis()->SetTitle((*iter_variable + "[GeV]").c_str());
    // }
    // else
    // {
    //     h3->GetXaxis()->SetTitle((*iter_variable).c_str());
    // }
    h3->GetXaxis()->SetTitleSize(20);
    h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(4.);
    h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(15);

    gPad->Update();
    // delete h1;
    // delete h2;
    // delete h3;
    // delete pad1;
    // delete pad2;

    // delete c;
}

ComparePlot::ComparePlot()
{
    m_input_file1 = nullptr;
    m_input_file2 = nullptr;
    m_input_directory1 = nullptr;
    m_input_directory2 = nullptr;
    m_output_file = nullptr;
    m_output_dir = nullptr;
    m_canvas_name = nullptr;
    m_h1 = nullptr;
    m_h2 = nullptr;
    m_c1 = nullptr;
    m_pad1 = nullptr;
    m_pad2 = nullptr;
    m_legend = nullptr;
    m_rebin_factor=nullptr;
    m_vec_CompareHistManager = vector<CompareHistManager *>();
    clog << "ComparePlot::ComparePlot: WARNING Default constructor, remember to initialize input and ouput files manually!" << endl;
    clog << "ComparePlot::ComparePlot: INFO Initialize" << endl;
}

ComparePlot::~ComparePlot()
{
    m_input_file1->Close();
    m_input_file2->Close();
    delete m_input_file1;
    delete m_input_file2;

    m_output_file->Close();
    delete m_output_file;

    for (auto iter_p = m_vec_CompareHistManager.begin(); iter_p != m_vec_CompareHistManager.end(); iter_p++)
    {
        cout << "ComparePlot:: ~ComparePlot: INFO delete CompareHistManager" << endl;
        delete *iter_p;
    }
    cout << "ComparePlot::~ComparePlot: Deleted" << endl;
}

ComparePlot::ComparePlot(const string &input_file1_name, const string &input_file2_name, const string &output_file_name)
{
    // For input file, read only!
    m_input_file1 = TFile::Open(input_file1_name.c_str(), "READ");
    m_input_file2 = TFile::Open(input_file2_name.c_str(), "READ");
    if (!m_input_file1)
    {
        clog << "ComparePlot::ComparePlot: ERROR input_file1 not exist" << endl;
        std::abort();
    }
    if (!m_input_file2)
    {
        clog << "ComparePlot::ComparePlot: ERROR input_file2 not exist" << endl;
        std::abort();
    }
    // Output file
    m_output_file = TFile::Open(output_file_name.c_str(), "UPDATE");
    if (!m_output_file)
    {
        clog << "ComparePlot::ComparePlot: ERROR Could not open output_file" << endl;
        std::abort();
    }

    m_input_directory1 = nullptr;
    m_input_directory2 = nullptr;
    m_output_file = nullptr;
    m_canvas_name = nullptr;
    m_h1 = nullptr;
    m_h2 = nullptr;
    m_c1 = nullptr;
    m_pad1 = nullptr;
    m_pad2 = nullptr;
    m_legend = nullptr;
    m_rebin_factor=nullptr;
    m_vec_CompareHistManager = vector<CompareHistManager *>();

    cout << "ComparePlot::ComparePlot: INFO Initialze with input and output files" << endl;
}

void ComparePlot::AddCompareHistManager(CompareHistManager *chm)
{
    m_vec_CompareHistManager.push_back(chm);
}

void ComparePlot::LoopOverCompareHistManager(float klambda)
{
    for(auto iter_chm = m_vec_CompareHistManager.begin(); iter_chm!=m_vec_CompareHistManager.end();iter_chm++)
    {
        // Get all containers
         if (m_vec_CompareHistManager.size() == 0)
    {
        clog << "" << endl;
        std::abort();
    }
    else
    {
        cout << ""
             << " " <<m_vec_CompareHistManager.size() << " CompareHistManager" << endl;
    }
    }
}

void ComparePlot::SetDefaultCompareHistManager()
{
}

string ComparePlot::TransformFloat(float KLambda)
{
    // input: any float KLambda
    // output: somethin like (string) 1p0

    // A kindly remind here: dure to the limit of std:to_string, this program cant handle
    // input more than 5 digit decimal
    // Transform input KLambda to string form, such as 1p00000
    string string_KLambda = std::to_string(KLambda);
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