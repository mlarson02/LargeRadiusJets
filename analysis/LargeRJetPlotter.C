#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TStyle.h"

void SetPlotStyle();
void mySmallText(Double_t x, Double_t y, Color_t color, char* text);

void styleHistogram(TH1F* hist, int color, const std::string& xTitle, const std::string& yTitle) {
    // Apply styling to the histogram
    hist->SetLineColor(color);
    hist->SetLineWidth(2);
    hist->SetTitle("");
    hist->GetXaxis()->SetTitle(xTitle.c_str());
    hist->GetYaxis()->SetTitle(yTitle.c_str());
    hist->GetXaxis()->SetTitleSize(0.04);
    hist->GetYaxis()->SetTitleSize(0.04);
    hist->GetXaxis()->SetLabelSize(0.03);
    hist->GetYaxis()->SetLabelSize(0.03);
}

void analyze_existing_histograms(const std::string& inputFileName, const std::string& outputPrefix) {
    // Open the input ROOT file
    TFile* inputFile = TFile::Open(inputFileName.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Could not open file " << inputFileName << std::endl;
        return;
    }

    // Retrieve histograms
    TH1F* h_Et = dynamic_cast<TH1F*>(inputFile->Get("h_et"));
    TH1F* h_eta = dynamic_cast<TH1F*>(inputFile->Get("h_eta"));
    TH1F* h_phi = dynamic_cast<TH1F*>(inputFile->Get("h_phi"));

    if (!h_Et || !h_eta || !h_phi) {
        std::cerr << "Error: Could not retrieve one or more histograms from the file." << std::endl;
        inputFile->Close();
        return;
    }

    // Normalize Et to unity
    if (h_Et->Integral() > 0) {
        h_Et->Scale(1.0 / h_Et->Integral());
    }

    // Style histograms
    styleHistogram(h_Et, kBlue, "Et (GeV)", "Counts");
    styleHistogram(h_eta, kRed, "#eta", "Counts");
    styleHistogram(h_phi, kGreen, "#phi (rad)", "Counts");

    SetPlotStyle();

    // Print overflow counts
    std::cout << "h_Et entries:" << h_Et->GetEntries();
    std::cout << "Overflow counts:" << std::endl;
    std::cout << "Et: " << h_Et->GetBinContent(h_Et->GetNbinsX() + 1) << std::endl;
    std::cout << "Eta: " << h_eta->GetBinContent(h_eta->GetNbinsX() + 1) << std::endl;
    std::cout << "Phi: " << h_phi->GetBinContent(h_phi->GetNbinsX() + 1) << std::endl;

    // Set canvas style
    gStyle->SetOptStat(0); // Turn off stats box
    gStyle->SetTitleFontSize(0.04);

    // Create canvases for each histogram and save to individual PDFs
    TCanvas* canvas = new TCanvas("canvas", "Histogram", 800, 600);

    // Et Histogram: Log scale and range adjustment
    canvas->SetLogx(); // Logarithmic x-axis
    canvas->SetLogy(); // Logarithmic y-axis
    // Set range for log scale and explicitly set unwanted bins to zero
    for (int bin = 1; bin <= h_Et->GetNbinsX(); ++bin) {
        double binCenter = h_Et->GetBinCenter(bin);
        if (binCenter < 1 || binCenter > 500) {
            h_Et->SetBinContent(bin, 0);
        }
    }
    h_Et->GetXaxis()->SetRangeUser(1, 500); // Set range for log scale
    h_Et->Draw("HIST");
    canvas->SaveAs((outputPrefix + "_Et.pdf").c_str());
    

    // Save Eta histogram
    canvas->cd();
    canvas->SetLogy(0); // Turn off log y-axis
    canvas->SetLogx(0); // Turn off log y-axis
    h_eta->Draw("HIST");
    canvas->SaveAs((outputPrefix + "_Eta.pdf").c_str());

    // Save Phi histogram
    canvas->cd();
    h_phi->Draw("HIST");
    canvas->SaveAs((outputPrefix + "_Phi.pdf").c_str());

    std::cout << "Histograms saved as separate PDFs with prefix: " << outputPrefix << std::endl;

    // Clean up
    inputFile->Close();
    delete canvas;
}

void callAnalyzeHistograms(const std::string& inputFileType) {
    // Usage: callAnalyzeHistograms("topo")
    std::string inputFile = "outputRootFiles/";
    std::string outputPrefix = "histograms/";

    if (inputFileType == "topo"){
        inputFile += "topo422.root";
        outputPrefix += "topo"; 
    }
    else if (inputFileType == "gfex"){
        inputFile += "gFex_smallrj.root";
        outputPrefix += "gFex"; 
    }
    if (inputFileType == "jfex"){
        inputFile += "jFex_smallrj.root";
        outputPrefix += "jFex"; 
    }

    analyze_existing_histograms(inputFile, outputPrefix);
}



// Declare helper functions 
void SetPlotStyle() {
  // from ATLAS plot style macro

  // use plain black on white colors
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetHistLineColor(1);

  gStyle->SetPalette(1);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20, 26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

  // use large fonts
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.07);
  gStyle->SetLabelFont(42, "x");
  gStyle->SetTitleFont(42, "x");
  gStyle->SetLabelFont(42, "y");
  gStyle->SetTitleFont(42, "y");
  gStyle->SetLabelFont(42, "z");
  gStyle->SetTitleFont(42, "z");
  gStyle->SetLabelSize(0.05, "x");
  gStyle->SetTitleSize(0.05, "x");
  gStyle->SetLabelSize(0.05, "y");
  gStyle->SetTitleSize(0.05, "y");
  gStyle->SetLabelSize(0.05, "z");
  gStyle->SetTitleSize(0.05, "z");

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2.);
  gStyle->SetLineStyleString(2, "[12 12]");

  // get rid of error bar caps
  gStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}

void mySmallText(Double_t x, Double_t y, Color_t color, char* text) {
  Double_t tsize = 0.044;
  TLatex l;
  l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x, y, text);
}
