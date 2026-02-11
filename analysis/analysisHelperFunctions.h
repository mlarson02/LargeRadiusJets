#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <bitset>
#include <array>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TSystem.h"
#include "THStack.h"
#include "TStyle.h"
#include "ROOT/RDataFrame.hxx" 
#include "TEllipse.h"
#include "TLatex.h"
#include "TProfile.h"
#include "TLine.h"
#include "TLegendEntry.h"
#include "TPaveText.h"
#include <regex>
#include <cmath>
#include <TMath.h>
#include <unordered_map>
#include <utility>
//#include "../algorithm/constants.h"


struct FileInfo {
    std::string inputObjectType;
    std::string seedObjectType;
    std::string rMergeValue;

    double jetRadiusSquared = 0.0;  // NEW FIELD
};

std::string getSampleTag(const std::string& path) {
    std::regex re("mc21_14TeV_(.+?)_e[0-9]+");
    std::smatch match;
    if (std::regex_search(path, match, re)) {
        return match[1];
    }
    return "";
}

std::string getProductionMode(const std::string& path) {
    std::string tag = getSampleTag(path);
    std::cout << "tag: " << tag << "\n";

    if (tag.find("vbf_novhh_cvv0") != std::string::npos) {
        return "VBF";
    }
    else if(tag.find("vbf_novhh_cvv1") != std::string::npos){
      return "VBF_SM";
    }
    else if (tag.find("flatpT_Zprime_tthad") != std::string::npos) {
        return "Zprime_tthad";
    }
    else if (tag.find("HHbbbb_HLLHC") != std::string::npos) {
        return "ggF";
    }
    else if (tag.find("ZvvH125_bb") != std::string::npos) {
        return "ZvvHbb";
    }
    else if (tag.find("ttbar_hdamp258p75_allhad") != std::string::npos) {
        return "ttbar_had";
    }
    else {
        return "null";
    }
}

FileInfo ParseFileName(const std::string& path)
{
    FileInfo info;

    const std::string rMergeTag = "_rMerge_";
    const std::string ioTag     = "_IO_";
    const std::string seedTag   = "_Seed_";
    const std::string r2Tag     = "_R2_";   // NEW TAG

    const std::string ext1      = ".root";
    const std::string ext2      = "_OR.root";

    // ----------------------------
    // Find tags
    // ----------------------------
    const size_t posRMerge = path.find(rMergeTag);
    const size_t posIO     = path.find(ioTag);
    const size_t posSeed   = path.find(seedTag);
    const size_t posR2     = path.find(r2Tag);

    if (posIO == std::string::npos || posSeed == std::string::npos || posSeed <= posIO) {
        std::cerr << "⚠️ ParseFileName: could not find expected IO/Seed tags in: " << path << "\n";
        return info;
    }

    // ----------------------------
    // Extract rMergeValue
    // ----------------------------
    if (posRMerge != std::string::npos) {
        size_t startR = posRMerge + rMergeTag.size();
        size_t endR = path.find('_', startR);
        if (endR == std::string::npos)
            endR = path.size();
        info.rMergeValue = path.substr(startR, endR - startR);
    }

    // ----------------------------
    // Extract jetRadiusSquared (value between _R2_ and _IO_)
    // ----------------------------
    if (posR2 != std::string::npos && posIO != std::string::npos && posR2 < posIO) {
        size_t startR2 = posR2 + r2Tag.size();
        size_t endR2 = posIO;

        std::string r2String = path.substr(startR2, endR2 - startR2);

        try {
            info.jetRadiusSquared = std::stod(r2String);
        } catch (...) {
            std::cerr << "⚠️ Failed to parse R2 value '" << r2String << "' as double\n";
        }
    }

    // ----------------------------
    // Extract inputObjectType
    // ----------------------------
    {
        const size_t startInput = posIO + ioTag.size();
        const size_t lenInput   = posSeed - startInput;
        std::string inputObjType;
        if(path.substr(startInput, lenInput) == "gepWTAConeCellsTowersJets") inputObjType = "ConeJets";
        else if(path.substr(startInput, lenInput) == "gepCellsTowers") inputObjType = "Towers";
        info.inputObjectType = inputObjType;
    }

    // ----------------------------
    // Extract seedObjectType
    // ----------------------------
    {
        size_t startSeed = posSeed + seedTag.size();

        // Find both possible extensions
        size_t end1 = path.find(ext1, startSeed);
        size_t end2 = path.find(ext2, startSeed);

        size_t endSeed = std::string::npos;
        if (end1 != std::string::npos && end2 != std::string::npos)
            endSeed = std::min(end1, end2);
        else if (end1 != std::string::npos)
            endSeed = end1;
        else if (end2 != std::string::npos)
            endSeed = end2;
        else
            endSeed = path.size();  // fallback

        info.seedObjectType = path.substr(startSeed, endSeed - startSeed);
    }

    return info;
}






// Return the point with the **largest efficiency (x)**
// among all points with rate y <= maxRateHz.
// If no point satisfies y <= maxRateHz, fall back to the point
// with y closest to maxRateHz (using log distance).
std::pair<double,double>
FindBestPointBelowRate(const TGraph *g, double maxRateHz)
{
    int n = g->GetN();
    const double *x = g->GetX();
    const double *y = g->GetY();

    double bestX = -1.0;
    double bestY =  0.0;

    // 1) Primary: best efficiency with y <= maxRateHz
    for (int i = 0; i < n; ++i) {
        if (y[i] <= maxRateHz && x[i] > bestX) {
            bestX = x[i];
            bestY = y[i];
        }
    }

    if (bestX >= 0.0) {
        return {bestX, bestY};
    }

    // 2) Fallback: if nothing is below the threshold,
    // choose the point closest in log(y) to maxRateHz.
    double bestDiff = 1e99;
    for (int i = 0; i < n; ++i) {
        double diff = std::fabs(std::log10(y[i]) - std::log10(maxRateHz));
        if (diff < bestDiff || (diff == bestDiff && x[i] > bestX)) {
            bestDiff = diff;
            bestX    = x[i];
            bestY    = y[i];
        }
    }
    return {bestX, bestY};
}



// ---------- Data container for one event ----------
struct EventDisplayInputs {
  TH2* heatmap = nullptr;  // e.g. sigTopo422Highest128SeedPositionsEvX
  std::vector<std::pair<double,double>> jFexSeeds;        // red, dashed
  std::vector<std::pair<double,double>> additionalSRJs;   // azure, dashed
  std::vector<std::pair<double,double>> newSeeds;         // green, solid
  std::vector<double>                  newSeedsEt;         // same size as newSeeds (for labels)
  TString outFile;                                         // output path
};

// Helper to create a TH2 with variable (or uniform) binning from the axes
static TH2F* MakeTH2Like(const char* name, const char* title,
                         const TAxis* axX, const TAxis* axY)
{
  const TArrayD& bx = *axX->GetXbins();
  const TArrayD& by = *axY->GetXbins();

  if (bx.GetSize() > 0 && by.GetSize() > 0)
    return new TH2F(name, title,
                    axX->GetNbins(), bx.GetArray(),
                    axY->GetNbins(), by.GetArray());
  if (bx.GetSize() > 0)
    return new TH2F(name, title,
                    axX->GetNbins(), bx.GetArray(),
                    axY->GetNbins(), axY->GetXmin(), axY->GetXmax());
  if (by.GetSize() > 0)
    return new TH2F(name, title,
                    axX->GetNbins(), axX->GetXmin(), axX->GetXmax(),
                    axY->GetNbins(), by.GetArray());
  return new TH2F(name, title,
                  axX->GetNbins(), axX->GetXmin(), axX->GetXmax(),
                  axY->GetNbins(), axY->GetXmin(), axY->GetXmax());
}



// Structure to store histograms used to generate background rate vs. signal efficiency plots
struct RateEffOut {
  TH1* hEff_vsThr;   // efficiency vs threshold (optional)
  TH1* hRate_vsThr;  // rate vs threshold in Hz (optional)
  TGraph* gRate_vsEff; // main output: y(rate) vs x(eff)
};

struct RateEff2DOut {
  TH2* hEff_vsThr_vsR;   // Signal efficiency surface vs (ET threshold, Rmax)
  TH2* hRate_vsThr_vsR;  // Background rate surface vs (ET threshold, Rmax)
  TGraphErrors* gRate_vsEff_all; // All (eff,rate) points from the 2D scan
};

// -----------------------------------------------------------------------------
// MakeRateVsEff_ScanRMax_AllowZeroR
//
// Like MakeRateVsEff_ScanRMax, but implements the logic:
//
//   pass if  (E_T > threshold) AND ( R < Rmax  OR  R == 0 )
//
// i.e. events with R == 0 are *only* subject to the E_T cut, not the R cut.
//
// Assumes the first R-bin (jb = 1) corresponds to R == 0 (or contains all R=0
// entries). If that's not strictly true, adjust the "zero bin" logic below.
// -----------------------------------------------------------------------------
RateEff2DOut MakeRateVsEff_ScanRMax_AllowZeroR(TH1* hSigEt, TH1* hBkgEt,
                                               TH1* hSigR,  TH1* hBkgR)
{
  // ---- (1) Cumulative along ET: pass if ET > threshold (HIGH -> LOW)
  auto* hSigCumEt = hSigEt->GetCumulative(/*forward=*/false,
                            (std::string(hSigEt->GetName())+"_cumul").c_str());
  auto* hBkgCumEt = hBkgEt->GetCumulative(/*forward=*/false,
                            (std::string(hBkgEt->GetName())+"_cumul").c_str());

  const int nbEt = hSigEt->GetNbinsX();
  const double totalSigEt = hSigEt->Integral(1, nbEt);

  std::vector<double> effEt(nbEt+1, 0.0), rateEt(nbEt+1, 0.0), errEt(nbEt+1, 0.0);
  for (int ib=1; ib<=nbEt; ++ib) {
    effEt[ib]  = (totalSigEt>0 ? hSigCumEt->GetBinContent(ib)/totalSigEt : 0.0);
    rateEt[ib] = hBkgCumEt->GetBinContent(ib);    // already in Hz if weighted
    errEt[ib]  = hBkgCumEt->GetBinError(ib);
  }

  // ---- (2) Cumulative along R, with special treatment for R == 0
  //
  // Logic at event level:
  //   pass if ET>thr AND [ (R == 0) OR (0 < R < Rmax) ]
  //
  // With factorization, the ET part is effEt[ib] / rateEt[ib].
  // The R part becomes:
  //
  //   fracSigR(jb) = f0_sig + (1 - f0_sig) * gSig(jb),
  //
  // where
  //   f0_sig = fraction of signal events with R == 0,
  //   gSig(jb) = fraction of *non-zero-R* events with R < Rmax(jb).
  //
  // Same for background.

  auto* hSigCumR_lo = hSigR->GetCumulative(/*forward=*/true,
                            (std::string(hSigR->GetName())+"_cumul_lo").c_str());
  auto* hBkgCumR_lo = hBkgR->GetCumulative(/*forward=*/true,
                            (std::string(hBkgR->GetName())+"_cumul_lo").c_str());

  const int nbR = hSigR->GetNbinsX();
  const double totalSigR = hSigR->Integral(1, nbR);
  const double totalBkgR = hBkgR->Integral(1, nbR); // Hz if weighted

  std::vector<double> fracSigR(nbR+1, 0.0);
  std::vector<double> fracBkgR(nbR+1, 0.0);

  if (totalSigR <= 0.0 || totalBkgR <= 0.0) {
    // Degenerate case: fall back to ET-only (no R cut at all)
    for (int jb=1; jb<=nbR; ++jb) {
      fracSigR[jb] = 1.0;
      fracBkgR[jb] = 1.0;
    }
  } else {
    // Assume bin 1 is the "R == 0" bin
    const int zeroBin = 1;

    const double sigR0 = hSigR->GetBinContent(zeroBin);
    const double bkgR0 = hBkgR->GetBinContent(zeroBin);

    const double f0_sig   = sigR0 / totalSigR;
    const double f0_bkg   = bkgR0 / totalBkgR;
    const double sigPos   = totalSigR - sigR0;  // R > 0
    const double bkgPos   = totalBkgR - bkgR0;  // R > 0

    for (int jb=1; jb<=nbR; ++jb) {
      const double cumSig = hSigCumR_lo->GetBinContent(jb); // includes zero bin
      const double cumBkg = hBkgCumR_lo->GetBinContent(jb);

      // Non-zero-R cumulative parts
      double gSig = 0.0;
      double gBkg = 0.0;

      if (sigPos > 0.0) {
        gSig = (cumSig - sigR0) / sigPos;  // fraction of non-zero-R with R < Rmax
        if (gSig < 0.0) gSig = 0.0;
        if (gSig > 1.0) gSig = 1.0;
      }

      if (bkgPos > 0.0) {
        gBkg = (cumBkg - bkgR0) / bkgPos;
        if (gBkg < 0.0) gBkg = 0.0;
        if (gBkg > 1.0) gBkg = 1.0;
      }

      // Total fraction passing the R logic: R == 0 OR (0 < R < Rmax)
      fracSigR[jb] = f0_sig + (1.0 - f0_sig) * gSig;
      fracBkgR[jb] = f0_bkg + (1.0 - f0_bkg) * gBkg;
    }
  }

  // ---- (3) Build 2D surfaces vs (ET_thr, Rmax)
  TH2F* hEff2D  = MakeTH2Like("hEff_vsThr_vsR_allowR0",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Signal (hh#rightarrow4b) Efficiency",
                     hSigEt->GetXaxis(), hSigR->GetXaxis());
  TH2F* hRate2D = MakeTH2Like("hRate_vsThr_vsR_allowR0",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Estimated Background Rate [Hz]",
                     hBkgEt->GetXaxis(), hBkgR->GetXaxis());

  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb) {
      const double eff  = std::clamp(effEt[ib]  * fracSigR[jb], 0.0, 1.0);
      const double rate = rateEt[ib] * fracBkgR[jb];
      const double err  = errEt[ib]  * fracBkgR[jb];

      hEff2D ->SetBinContent(ib, jb, eff);
      hRate2D->SetBinContent(ib, jb, rate);
      hRate2D->SetBinError  (ib, jb, err);
    }
  }

  // ---- (4) Collect all (eff, rate) points into a TGraphErrors
  const int npts = nbEt * nbR;
  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_all_allowR0");
  g->SetTitle("Trigger Rate vs Signal Efficiency;Signal (hh#rightarrow4b) Efficiency;Estimated Background Rate [Hz]");

  int k = 0;
  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb, ++k) {
      const double eff  = hEff2D ->GetBinContent(ib,jb);
      const double rate = hRate2D->GetBinContent(ib,jb);
      const double err  = hRate2D->GetBinError  (ib,jb);
      g->SetPoint(k, eff, rate);
      g->SetPointError(k, 0.0, err);
    }
  }

  return {hEff2D, hRate2D, g};
}

// -----------------------------------------------------------------------------
// MakeRateVsEff_ScanRWindow
// Scan ET threshold and an R window cut simultaneously.
// Pass condition in R: R_min <= R <= R_max.
// We scan over all bin pairs (jMin, jMax) with jMin <= jMax.
//
// Inputs:
//   hSigEt, hBkgEt : 1D ET distributions (sig counts, bkg weighted to Hz)
//   hSigR,  hBkgR  : 1D R distributions
//
// Output: here I focus on building a TGraphErrors with all (eff, rate) points.
// You can wrap it in your own struct, e.g. RateEffWindowOut.
// -----------------------------------------------------------------------------
TGraphErrors* MakeRateVsEff_ScanRWindow(TH1* hSigEt, TH1* hBkgEt,
                                        TH1* hSigR,  TH1* hBkgR)
{
  // ---- (1) Cumulative along ET: pass if ET > threshold (HIGH -> LOW)
  auto* hSigCumEt = hSigEt->GetCumulative(/*forward=*/false,
                            (std::string(hSigEt->GetName())+"_cumulEt").c_str());
  auto* hBkgCumEt = hBkgEt->GetCumulative(/*forward=*/false,
                            (std::string(hBkgEt->GetName())+"_cumulEt").c_str());

  const int nbEt = hSigEt->GetNbinsX();
  const double totalSigEt = hSigEt->Integral(1, nbEt);

  std::vector<double> effEt(nbEt+1, 0.0), rateEt(nbEt+1, 0.0), errEt(nbEt+1, 0.0);
  for (int ib=1; ib<=nbEt; ++ib) {
    effEt[ib]  = (totalSigEt > 0 ? hSigCumEt->GetBinContent(ib) / totalSigEt : 0.0);
    rateEt[ib] = hBkgCumEt->GetBinContent(ib);    // already in Hz if weighted
    errEt[ib]  = hBkgCumEt->GetBinError(ib);
  }

  // ---- (2) Cumulative along R from LOW -> HIGH
  // We will use this to build arbitrary windows [jMin, jMax].
  auto* hSigCumR_lo = hSigR->GetCumulative(/*forward=*/true,
                            (std::string(hSigR->GetName())+"_cumul_lo").c_str());
  auto* hBkgCumR_lo = hBkgR->GetCumulative(/*forward=*/true,
                            (std::string(hBkgR->GetName())+"_cumul_lo").c_str());

  const int nbR  = hSigR->GetNbinsX();
  const double totalSigR = hSigR->Integral(1, nbR);
  const double totalBkgR = hBkgR->Integral(1, nbR); // Hz if weighted

  // Pre-store the cumulative contents for convenience
  std::vector<double> cumSigR(nbR+2, 0.0), cumBkgR(nbR+2, 0.0);
  for (int jb=1; jb<=nbR; ++jb) {
    cumSigR[jb] = hSigCumR_lo->GetBinContent(jb);
    cumBkgR[jb] = hBkgCumR_lo->GetBinContent(jb);
  }
  // Define "bin 0" cumulative as 0 so we can safely do cum(jMax)-cum(jMin-1)
  cumSigR[0] = 0.0;
  cumBkgR[0] = 0.0;

  // ---- (3) Number of (R_min, R_max) windows and total points
  const int nWindows = nbR * (nbR + 1) / 2; // all pairs jMin <= jMax
  const int npts     = nbEt * nWindows;

  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_window");
  g->SetTitle("Trigger Rate vs Signal Efficiency (R window);"
              "Signal (hh#rightarrow4b) Efficiency;Estimated Background Rate [Hz]");

  int k = 0;
  for (int ib=1; ib<=nbEt; ++ib) {

    const double effEt_ib  = effEt[ib];
    const double rateEt_ib = rateEt[ib];
    const double errEt_ib  = errEt[ib];

    // Loop over all windows in R: jMin <= jMax
    for (int jMin=1; jMin<=nbR; ++jMin) {
      for (int jMax=jMin; jMax<=nbR; ++jMax, ++k) {

        // Fraction of events in [R(jMin), R(jMax)] for signal and background
        double winSig   = cumSigR[jMax] - cumSigR[jMin-1];
        double winBkg   = cumBkgR[jMax] - cumBkgR[jMin-1];

        double fracSigR = (totalSigR > 0 ? winSig / totalSigR : 0.0);
        double fracBkgR = (totalBkgR > 0 ? winBkg / totalBkgR : 0.0);

        const double eff  = std::clamp(effEt_ib * fracSigR, 0.0, 1.0);
        const double rate = rateEt_ib * fracBkgR;
        const double err  = errEt_ib * fracBkgR;

        g->SetPoint(k, eff, rate);
        g->SetPointError(k, 0.0, err);

        // If you'd like to keep track of which window it was, you can encode it
        // in the point index or in auxiliary arrays (R_min, R_max) stored elsewhere.
      }
    }
  }

  return g;
}

// -----------------------------------------------------------------------------
// MakeRateVsEff_ScanRMin
// Scan ET threshold and R_min simultaneously using 1D histograms.
// Inputs:
//   hSigEt, hBkgEt : 1D ET distributions (sig counts, bkg weighted to Hz)
//   hSigR,  hBkgR  : 1D R=psi_lead/psi_sublead distributions
// Returns 2D efficiency / rate surfaces and a graph with all (eff, rate) pairs.
// Now the R cut is: pass if R > R_min.
// -----------------------------------------------------------------------------
/*
RateEff2DOut MakeRateVsEff_ScanRMin(TH1* hSigEt, TH1* hBkgEt,
                                    TH1* hSigR,  TH1* hBkgR)
{
  // ---- (1) Cumulative along ET: pass if ET > threshold (HIGH -> LOW)
  auto* hSigCumEt = hSigEt->GetCumulative(false,
                            (std::string(hSigEt->GetName())+"_cumulEt").c_str());
  auto* hBkgCumEt = hBkgEt->GetCumulative(false,
                            (std::string(hBkgEt->GetName())+"_cumulEt").c_str());

  const int nbEt = hSigEt->GetNbinsX();
  const double totalSigEt = hSigEt->Integral(1, nbEt);

  std::vector<double> effEt(nbEt+1, 0.0), rateEt(nbEt+1, 0.0), errEt(nbEt+1, 0.0);
  for (int ib=1; ib<=nbEt; ++ib) {
    effEt[ib]  = (totalSigEt>0 ? hSigCumEt->GetBinContent(ib)/totalSigEt : 0.0);
    rateEt[ib] = hBkgCumEt->GetBinContent(ib);    // already in Hz if weighted
    errEt[ib]  = hBkgCumEt->GetBinError(ib);
  }

  // ---- (2) Cumulative along R: pass if R > R_min (HIGH -> LOW)
  auto* hSigCumR_hi = hSigR->GetCumulative(false,
                            (std::string(hSigR->GetName())+"_cumul_hi").c_str());
  auto* hBkgCumR_hi = hBkgR->GetCumulative(false,
                            (std::string(hBkgR->GetName())+"_cumul_hi").c_str());

  const int nbR  = hSigR->GetNbinsX();
  const double totalSigR = hSigR->Integral(1, nbR);
  const double totalBkgR = hBkgR->Integral(1, nbR); // Hz if weighted

  std::vector<double> fracSigR(nbR+1, 0.0), fracBkgR(nbR+1, 0.0);
  for (int jb=1; jb<=nbR; ++jb) {
    // Now each bin jb represents the fraction with R >= R_bin_center(jb)
    fracSigR[jb] = (totalSigR>0 ? hSigCumR_hi->GetBinContent(jb)/totalSigR : 0.0);
    fracBkgR[jb] = (totalBkgR>0 ? hBkgCumR_hi->GetBinContent(jb)/totalBkgR : 0.0);
  }

  // ---- (3) Build 2D surfaces vs (ET_thr, R_min)
  TH2F* hEff2D  = MakeTH2Like("hEff_vsThr_vsRmin",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{min};Signal (hh#rightarrow4b) Efficiency",
                     hSigEt->GetXaxis(), hSigR->GetXaxis());
  TH2F* hRate2D = MakeTH2Like("hRate_vsThr_vsRmin",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{min};Estimated Background Rate [Hz]",
                     hBkgEt->GetXaxis(), hBkgR->GetXaxis());

  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb) {
      const double eff  = std::clamp(effEt[ib]  * fracSigR[jb], 0.0, 1.0);
      const double rate = rateEt[ib] * fracBkgR[jb];
      hEff2D ->SetBinContent(ib, jb, eff);
      hRate2D->SetBinContent(ib, jb, rate);
      // (Optional) error on rate:
      hRate2D->SetBinError(ib, jb, errEt[ib] * fracBkgR[jb]);
    }
  }

  // ---- (4) Collect all (eff, rate) points into a TGraphErrors
  const int npts = nbEt * nbR;
  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_all_Rmin");
  g->SetTitle("Trigger Rate vs Signal Efficiency;Signal (hh#rightarrow4b) Efficiency;Estimated Background Rate [Hz]");

  int k=0;
  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb, ++k) {
      const double eff  = hEff2D ->GetBinContent(ib,jb);
      const double rate = hRate2D->GetBinContent(ib,jb);
      const double err  = hRate2D->GetBinError  (ib,jb);
      g->SetPoint(k, eff, rate);
      g->SetPointError(k, 0.0, err);
    }
  }

  return {hEff2D, hRate2D, g};
}
*/
// -----------------------------------------------------------------------------
// MakeRateVsEff_ScanRMin  (2D version)
// Scan ET threshold and R_min simultaneously using 2D histograms.
//
// Inputs:
//   hSigEtR, hBkgEtR : 2D ET–R distributions
//                      X axis = Leading LRJ E_T
//                      Y axis = R (e.g. tau2/tau1 product)
//
//   Event passes if:   ET > ET_thr(bin_x)  AND  R >= R_min(bin_y)
//
// Returns 2D efficiency / rate surfaces and a graph with all (eff, rate) pairs.
// -----------------------------------------------------------------------------
RateEff2DOut MakeRateVsEff_ScanRMin(TH2* hSigEtR, TH2* hBkgEtR)
{
  // --- basic checks ---------------------------------------------------------
  if (!hSigEtR || !hBkgEtR) {
    throw std::runtime_error("MakeRateVsEff_ScanRMin: null input histogram");
  }

  const int nbEt = hSigEtR->GetNbinsX();
  const int nbR  = hSigEtR->GetNbinsY();

  if (nbEt != hBkgEtR->GetNbinsX() || nbR != hBkgEtR->GetNbinsY()) {
    throw std::runtime_error("MakeRateVsEff_ScanRMin: signal/bkg 2D hist binning mismatch");
  }

  // Total signal for normalisation (all ET, all R)
  const double totalSig = hSigEtR->Integral(1, nbEt, 1, nbR);

  // --- (1) Cumulative sums over region (ET >= thr_x, R >= R_min_y) ---------
  //
  // We want cum(ix,iy) = sum_{x>=ix, y>=iy} H(x,y).
  //
  // Recurrence (summing from high to low indices in both x and y):
  //   C(ix,iy) = H(ix,iy) + C(ix+1,iy) + C(ix,iy+1) - C(ix+1,iy+1)

  std::vector<std::vector<double>> cumSig   (nbEt+2, std::vector<double>(nbR+2, 0.0));
  std::vector<std::vector<double>> cumBkg   (nbEt+2, std::vector<double>(nbR+2, 0.0));
  std::vector<std::vector<double>> cumBkgVar(nbEt+2, std::vector<double>(nbR+2, 0.0));

  for (int ix = nbEt; ix >= 1; --ix) {
    for (int iy = nbR; iy >= 1; --iy) {
      const double s    = hSigEtR->GetBinContent(ix, iy);
      const double b    = hBkgEtR->GetBinContent(ix, iy);
      const double berr = hBkgEtR->GetBinError  (ix, iy);
      const double var  = berr * berr;

      cumSig[ix][iy] =
          s + cumSig[ix+1][iy] + cumSig[ix][iy+1] - cumSig[ix+1][iy+1];

      cumBkg[ix][iy] =
          b + cumBkg[ix+1][iy] + cumBkg[ix][iy+1] - cumBkg[ix+1][iy+1];

      cumBkgVar[ix][iy] =
          var + cumBkgVar[ix+1][iy] + cumBkgVar[ix][iy+1] - cumBkgVar[ix+1][iy+1];
    }
  }

  // --- (2) Output 2D histograms (same binning as inputs) -------------------
  TH2F* hEff2D = MakeTH2Like(
      "hEff_vsThr_vsRmin",
      ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{min};Signal (hh#rightarrow4b) Efficiency",
      hSigEtR->GetXaxis(), hSigEtR->GetYaxis());

  TH2F* hRate2D = MakeTH2Like(
      "hRate_vsThr_vsRmin",
      ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{min};Estimated Background Rate [Hz]",
      hBkgEtR->GetXaxis(), hBkgEtR->GetYaxis());

  for (int ix = 1; ix <= nbEt; ++ix) {
    for (int iy = 1; iy <= nbR; ++iy) {
      const double sigPass = cumSig[ix][iy];     // ET >= ix, R >= iy
      const double bkgPass = cumBkg[ix][iy];
      const double varPass = cumBkgVar[ix][iy];

      const double eff  = (totalSig > 0.0 ? sigPass / totalSig : 0.0);
      const double rate = bkgPass;
      const double err  = (varPass > 0.0 ? std::sqrt(varPass) : 0.0);

      hEff2D ->SetBinContent(ix, iy, eff);
      hRate2D->SetBinContent(ix, iy, rate);
      hRate2D->SetBinError  (ix, iy, err);
    }
  }

  // --- (3) Collect all (eff, rate) points into a TGraphErrors -------------
  const int npts = nbEt * nbR;
  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_all_Rmin");
  g->SetTitle("Trigger Rate vs Signal Efficiency;"
              "Signal (hh#rightarrow4b) Efficiency;"
              "Estimated Background Rate [Hz]");

  int k = 0;
  for (int ix = 1; ix <= nbEt; ++ix) {
    for (int iy = 1; iy <= nbR; ++iy, ++k) {
      const double eff  = hEff2D ->GetBinContent(ix, iy);
      const double rate = hRate2D->GetBinContent(ix, iy);
      const double err  = hRate2D->GetBinError  (ix, iy);

      g->SetPoint     (k, eff, rate);
      g->SetPointError(k, 0.0, err);
    }
  }

  return {hEff2D, hRate2D, g};
}

struct RateEff3DOut {
  TH3* hEff_vsThr3D;          // Signal efficiency vs (x_thr, y_thr, z_thr)
  TH3* hRate_vsThr3D;         // Background rate vs (x_thr, y_thr, z_thr)
  TGraphErrors* gRate_vsEff_all;       // all (eff,rate) points
  TGraphErrors* gRate_vsEff_frontier;  // only best points (efficiency frontier)
};


TH3F* MakeTH3Like(const char* name,
                  const char* title,
                  const TAxis* xaxis,
                  const TAxis* yaxis,
                  const TAxis* zaxis)
{
  auto* h = new TH3F(
      name, title,
      xaxis->GetNbins(), xaxis->GetXmin(), xaxis->GetXmax(),
      yaxis->GetNbins(), yaxis->GetXmin(), yaxis->GetXmax(),
      zaxis->GetNbins(), zaxis->GetXmin(), zaxis->GetXmax());
  return h;
}


RateEff3DOut MakeRateVsEff_Scan3DMin(TH3* hSigXYZ, TH3* hBkgXYZ)
{
  // --- basic checks ---------------------------------------------------------
  if (!hSigXYZ || !hBkgXYZ) {
    throw std::runtime_error("MakeRateVsEff_Scan3DMin: null input histogram");
  }

  const int nbX = hSigXYZ->GetNbinsX();
  const int nbY = hSigXYZ->GetNbinsY();
  const int nbZ = hSigXYZ->GetNbinsZ();

  if (nbX != hBkgXYZ->GetNbinsX() ||
      nbY != hBkgXYZ->GetNbinsY() ||
      nbZ != hBkgXYZ->GetNbinsZ()) {
    throw std::runtime_error("MakeRateVsEff_Scan3DMin: signal/bkg 3D hist binning mismatch");
  }

  // Total signal for normalisation (all x,y,z)
  const double totalSig = hSigXYZ->Integral(1, nbX, 1, nbY, 1, nbZ);

  // --- (1) Cumulative sums over region (x>=ix, y>=iy, z>=iz) ---------------
  //
  // We want cum(ix,iy,iz) = sum_{x>=ix, y>=iy, z>=iz} H(x,y,z).
  //
  // Recurrence (summing from high to low indices in x, y, z):
  //   C(ix,iy,iz) = H(ix,iy,iz)
  //               + C(ix+1,iy,  iz  ) + C(ix,  iy+1,iz  ) + C(ix,  iy,  iz+1)
  //               - C(ix+1,iy+1,iz  ) - C(ix+1,iy,  iz+1) - C(ix,  iy+1,iz+1)
  //               + C(ix+1,iy+1,iz+1)

  const int NX = nbX + 2;
  const int NY = nbY + 2;
  const int NZ = nbZ + 2;

  auto idx = [NY, NZ](int ix, int iy, int iz) {
    return (ix * NY + iy) * NZ + iz;
  };

  std::vector<double> cumSig   (NX * NY * NZ, 0.0);
  std::vector<double> cumBkg   (NX * NY * NZ, 0.0);
  std::vector<double> cumBkgVar(NX * NY * NZ, 0.0);

  for (int ix = nbX; ix >= 1; --ix) {
    for (int iy = nbY; iy >= 1; --iy) {
      for (int iz = nbZ; iz >= 1; --iz) {
        const double s    = hSigXYZ->GetBinContent(ix, iy, iz);
        const double b    = hBkgXYZ->GetBinContent(ix, iy, iz);
        const double berr = hBkgXYZ->GetBinError  (ix, iy, iz);
        const double var  = berr * berr;

        const int i000 = idx(ix,   iy,   iz  );
        const int i100 = idx(ix+1, iy,   iz  );
        const int i010 = idx(ix,   iy+1, iz  );
        const int i001 = idx(ix,   iy,   iz+1);
        const int i110 = idx(ix+1, iy+1, iz  );
        const int i101 = idx(ix+1, iy,   iz+1);
        const int i011 = idx(ix,   iy+1, iz+1);
        const int i111 = idx(ix+1, iy+1, iz+1);

        cumSig[i000] =
            s + cumSig[i100] + cumSig[i010] + cumSig[i001]
              - cumSig[i110] - cumSig[i101] - cumSig[i011]
              + cumSig[i111];

        cumBkg[i000] =
            b + cumBkg[i100] + cumBkg[i010] + cumBkg[i001]
              - cumBkg[i110] - cumBkg[i101] - cumBkg[i011]
              + cumBkg[i111];

        cumBkgVar[i000] =
            var + cumBkgVar[i100] + cumBkgVar[i010] + cumBkgVar[i001]
                - cumBkgVar[i110] - cumBkgVar[i101] - cumBkgVar[i011]
                + cumBkgVar[i111];
      }
    }
  }

  // --- (2) Output 3D histograms (same binning as inputs) -------------------
  TH3F* hEff3D = MakeTH3Like(
      "hEff_vsThr3D",
      ";X threshold;Y threshold;Z threshold;Signal Efficiency",
      hSigXYZ->GetXaxis(), hSigXYZ->GetYaxis(), hSigXYZ->GetZaxis());

  TH3F* hRate3D = MakeTH3Like(
      "hRate_vsThr3D",
      ";X threshold;Y threshold;Z threshold;Estimated Background Rate [Hz]",
      hBkgXYZ->GetXaxis(), hBkgXYZ->GetYaxis(), hBkgXYZ->GetZaxis());

  for (int ix = 1; ix <= nbX; ++ix) {
    for (int iy = 1; iy <= nbY; ++iy) {
      for (int iz = 1; iz <= nbZ; ++iz) {
        const double sigPass = cumSig   [idx(ix, iy, iz)];  // x>=ix,y>=iy,z>=iz
        const double bkgPass = cumBkg   [idx(ix, iy, iz)];
        const double varPass = cumBkgVar[idx(ix, iy, iz)];

        const double eff  = (totalSig > 0.0 ? sigPass / totalSig : 0.0);
        const double rate = bkgPass;
        const double err  = (varPass > 0.0 ? std::sqrt(varPass) : 0.0);

        hEff3D ->SetBinContent(ix, iy, iz, eff);
        hRate3D->SetBinContent(ix, iy, iz, rate);
        hRate3D->SetBinError  (ix, iy, iz, err);
      }
    }
  }

  // --- (3) Collect all (eff, rate) points into a TGraphErrors -------------
  const int npts = nbX * nbY * nbZ;
  auto* gAll = new TGraphErrors(npts);
  gAll->SetName("gRate_vsEff_all_3D");
  gAll->SetTitle("Trigger Rate vs Signal Efficiency;"
                 "Signal Efficiency;"
                 "Estimated Background Rate [Hz]");

  struct Point {
    double eff;
    double rate;
    double err;
  };
  std::vector<Point> points;
  points.reserve(npts);

  int k = 0;
  for (int ix = 1; ix <= nbX; ++ix) {
    for (int iy = 1; iy <= nbY; ++iy) {
      for (int iz = 1; iz <= nbZ; ++iz, ++k) {
        const double eff  = hEff3D ->GetBinContent(ix, iy, iz);
        const double rate = hRate3D->GetBinContent(ix, iy, iz);
        const double err  = hRate3D->GetBinError  (ix, iy, iz);

        gAll->SetPoint     (k, eff, rate);
        gAll->SetPointError(k, 0.0, err);

        points.push_back({eff, rate, err});
      }
    }
  }

  // --- (4) Build the frontier: best efficiency for each rate ---------------
  //
  // Sort by rate ascending, and for equal rate, by efficiency descending.
  std::sort(points.begin(), points.end(),
            [](const Point& a, const Point& b) {
              if (a.rate < b.rate) return true;
              if (a.rate > b.rate) return false;
              return a.eff > b.eff; // for same rate, keep higher eff first
            });

  std::vector<Point> frontier;
  frontier.reserve(points.size());

  double bestEffSoFar = -1.0;
  const double eps = 1e-8;

  for (const auto& p : points) {
    // keep only points that improve the efficiency at this or higher rate
    if (p.eff > bestEffSoFar + eps) {
      frontier.push_back(p);
      bestEffSoFar = p.eff;
    }
  }

  auto* gFrontier = new TGraphErrors(frontier.size());
  gFrontier->SetName("gRate_vsEff_3D_frontier");
  gFrontier->SetTitle("Trigger Rate vs Signal Efficiency (3D scan frontier);"
                      "Signal (hh#rightarrow4b) Efficiency;"
                      "Estimated Background Rate [Hz]");

  for (size_t i = 0; i < frontier.size(); ++i) {
    gFrontier->SetPoint(i, frontier[i].eff, frontier[i].rate);
    gFrontier->SetPointError(i, 0.0, frontier[i].err);
  }

  RateEff3DOut out;
  out.hEff_vsThr3D          = hEff3D;
  out.hRate_vsThr3D         = hRate3D;
  out.gRate_vsEff_all       = gAll;
  out.gRate_vsEff_frontier  = gFrontier;
  return out;
}


// -----------------------------------------------------------------------------
// MakeRateVsEff_ScanRMax
// Scan ET threshold and Rmax simultaneously using 1D histograms.
// Inputs:
//   hSigEt, hBkgEt : 1D ET distributions (sig counts, bkg weighted to Hz)
//   hSigR,  hBkgR  : 1D R=psi_lead/psi_sublead distributions
// Returns 2D efficiency / rate surfaces and a graph with all (eff, rate) pairs.
// -----------------------------------------------------------------------------
/*
RateEff2DOut MakeRateVsEff_ScanRMax(TH1* hSigEt, TH1* hBkgEt,
                                    TH1* hSigR,  TH1* hBkgR)
{
  // ---- (1) Cumulative along ET: pass if ET > threshold (HIGH -> LOW)
  auto* hSigCumEt = hSigEt->GetCumulative(false,
                            (std::string(hSigEt->GetName())+"_cumul").c_str());
  auto* hBkgCumEt = hBkgEt->GetCumulative(false,
                            (std::string(hBkgEt->GetName())+"_cumul").c_str());

  const int nbEt = hSigEt->GetNbinsX();
  const double totalSigEt = hSigEt->Integral(1, nbEt);

  std::vector<double> effEt(nbEt+1, 0.0), rateEt(nbEt+1, 0.0), errEt(nbEt+1, 0.0);
  for (int ib=1; ib<=nbEt; ++ib) {
    effEt[ib]  = (totalSigEt>0 ? hSigCumEt->GetBinContent(ib)/totalSigEt : 0.0);
    rateEt[ib] = hBkgCumEt->GetBinContent(ib);    // already in Hz if weighted
    errEt[ib]  = hBkgCumEt->GetBinError(ib);
  }

  // ---- (2) Cumulative along R: pass if R < Rmax (LOW -> HIGH)
  auto* hSigCumR_lo = hSigR->GetCumulative(true,
                            (std::string(hSigR->GetName())+"_cumul_lo").c_str());
  auto* hBkgCumR_lo = hBkgR->GetCumulative(true,
                            (std::string(hBkgR->GetName())+"_cumul_lo").c_str());

  const int nbR  = hSigR->GetNbinsX();
  const double totalSigR = hSigR->Integral(1, nbR);
  const double totalBkgR = hBkgR->Integral(1, nbR); // Hz if weighted

  std::vector<double> fracSigR(nbR+1, 0.0), fracBkgR(nbR+1, 0.0);
  for (int jb=1; jb<=nbR; ++jb) {
    fracSigR[jb] = (totalSigR>0 ? hSigCumR_lo->GetBinContent(jb)/totalSigR : 0.0);
    fracBkgR[jb] = (totalBkgR>0 ? hBkgCumR_lo->GetBinContent(jb)/totalBkgR : 0.0);
  }

  // ---- (3) Build 2D surfaces vs (ET_thr, Rmax)
  TH2F* hEff2D  = MakeTH2Like("hEff_vsThr_vsR",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Signal (hh#rightarrow4b) Efficiency",
                     hSigEt->GetXaxis(), hSigR->GetXaxis());
  TH2F* hRate2D = MakeTH2Like("hRate_vsThr_vsR",
                     ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Estimated Background Rate [Hz]",
                     hBkgEt->GetXaxis(), hBkgR->GetXaxis());

  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb) {
      const double eff  = std::clamp(effEt[ib]  * fracSigR[jb], 0.0, 1.0);
      const double rate = rateEt[ib] * fracBkgR[jb];
      hEff2D ->SetBinContent(ib, jb, eff);
      hRate2D->SetBinContent(ib, jb, rate);
      // (Optional) error on rate:
      hRate2D->SetBinError(ib, jb, errEt[ib] * fracBkgR[jb]);
    }
  }

  // ---- (4) Collect all (eff, rate) points into a TGraphErrors
  const int npts = nbEt * nbR;
  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_all");
  g->SetTitle("Trigger Rate vs Signal Efficiency;Signal (hh#rightarrow4b) Efficiency;Estimated Background Rate [Hz]");

  int k=0;
  for (int ib=1; ib<=nbEt; ++ib) {
    for (int jb=1; jb<=nbR; ++jb, ++k) {
      const double eff  = hEff2D ->GetBinContent(ib,jb);
      const double rate = hRate2D->GetBinContent(ib,jb);
      const double err  = hRate2D->GetBinError  (ib,jb);
      g->SetPoint(k, eff, rate);
      g->SetPointError(k, 0.0, err);
    }
  }

  return {hEff2D, hRate2D, g};
}*/
// 2D version: hSigEtR, hBkgEtR are TH2 with X=ET, Y=R
RateEff2DOut MakeRateVsEff_ScanRMax(TH2* hSigEtR, TH2* hBkgEtR)
{
  // --- basic checks ---------------------------------------------------------
  if (!hSigEtR || !hBkgEtR) {
    throw std::runtime_error("MakeRateVsEff_ScanRMax: null input histogram");
  }
  const int nbEt = hSigEtR->GetNbinsX();
  const int nbR  = hSigEtR->GetNbinsY();

  if (nbEt != hBkgEtR->GetNbinsX() || nbR != hBkgEtR->GetNbinsY()) {
    throw std::runtime_error("MakeRateVsEff_ScanRMax: signal/bkg 2D hist binning mismatch");
  }

  // Total signal for normalisation (all ET, all R)
  const double totalSig = hSigEtR->Integral(1, nbEt, 1, nbR);

  // --- (1) Build cumulative sums over (ET_thr, Rmax) -----------------------
  //
  // We want cum(ix,iy) = sum_{x>=ix, y<=iy} H(x,y).
  // Do this with a 2D prefix-sum style recurrence:
  //   C(ix,iy) = H(ix,iy) + C(ix+1,iy) + C(ix,iy-1) - C(ix+1,iy-1)

  std::vector<std::vector<double>> cumSig(nbEt+2, std::vector<double>(nbR+2, 0.0));
  std::vector<std::vector<double>> cumBkg(nbEt+2, std::vector<double>(nbR+2, 0.0));
  std::vector<std::vector<double>> cumBkgVar(nbEt+2, std::vector<double>(nbR+2, 0.0));

  for (int ix = nbEt; ix >= 1; --ix) {
    for (int iy = 1; iy <= nbR; ++iy) {
      const double s    = hSigEtR->GetBinContent(ix, iy);
      const double b    = hBkgEtR->GetBinContent(ix, iy);
      const double berr = hBkgEtR->GetBinError  (ix, iy);

      cumSig[ix][iy] =
          s + cumSig[ix+1][iy] + cumSig[ix][iy-1] - cumSig[ix+1][iy-1];

      cumBkg[ix][iy] =
          b + cumBkg[ix+1][iy] + cumBkg[ix][iy-1] - cumBkg[ix+1][iy-1];

      // store variance for error propagation
      const double var = berr * berr;
      cumBkgVar[ix][iy] =
          var + cumBkgVar[ix+1][iy] + cumBkgVar[ix][iy-1] - cumBkgVar[ix+1][iy-1];
    }
  }

  // --- (2) Create output 2D histograms (same binning as input) ------------
  TH2F* hEff2D  = MakeTH2Like(
      "hEff_vsThr_vsR",
      ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Signal (hh#rightarrow4b) Efficiency",
      hSigEtR->GetXaxis(), hSigEtR->GetYaxis());

  TH2F* hRate2D = MakeTH2Like(
      "hRate_vsThr_vsR",
      ";Leading JetTagger LRJ E_{T} threshold [GeV];R_{max};Estimated Background Rate [Hz]",
      hBkgEtR->GetXaxis(), hBkgEtR->GetYaxis());

  for (int ix = 1; ix <= nbEt; ++ix) {
    for (int iy = 1; iy <= nbR; ++iy) {
      const double sigPass = cumSig[ix][iy];      // ET >= ix, R <= iy
      const double bkgPass = cumBkg[ix][iy];
      const double varPass = cumBkgVar[ix][iy];

      const double eff  = (totalSig > 0.0 ? sigPass / totalSig : 0.0);
      const double rate = bkgPass;
      const double err  = (varPass > 0.0 ? std::sqrt(varPass) : 0.0);

      hEff2D ->SetBinContent(ix, iy, eff);
      hRate2D->SetBinContent(ix, iy, rate);
      hRate2D->SetBinError  (ix, iy, err);
    }
  }

  // --- (3) Collect all (eff, rate) points into a TGraphErrors -------------
  const int npts = nbEt * nbR;
  auto* g = new TGraphErrors(npts);
  g->SetName("gRate_vsEff_all");
  g->SetTitle("Trigger Rate vs Signal Efficiency;"
              "Signal (hh#rightarrow4b) Efficiency;"
              "Estimated Background Rate [Hz]");

  int k = 0;
  for (int ix = 1; ix <= nbEt; ++ix) {
    for (int iy = 1; iy <= nbR; ++iy, ++k) {
      const double eff  = hEff2D ->GetBinContent(ix, iy);
      const double rate = hRate2D->GetBinContent(ix, iy);
      const double err  = hRate2D->GetBinError  (ix, iy);

      g->SetPoint     (k, eff, rate);
      g->SetPointError(k, 0.0, err);
    }
  }

  return {hEff2D, hRate2D, g};
}


// Function to generate background rate vs. signal efficiency plots
RateEffOut MakeRateVsEff(TH1* hSig, TH1* hBkg) {
  // 1) cumulative from HIGH -> LOW threshold
  auto* hSigCum = hSig->GetCumulative(/*forward=*/false, (std::string(hSig->GetName())+"_cumul").c_str());
  auto* hBkgCum = hBkg->GetCumulative(/*forward=*/false, (std::string(hBkg->GetName())+"_cumul").c_str());

  // 2) signal efficiency vs threshold
  const int nb = hSig->GetNbinsX();
  const double totalSig = hSig->Integral(1, nb);  // unweighted total signal events
  auto* hEff = (TH1*)hSigCum->Clone((std::string(hSig->GetName())+"_eff_vs_thr").c_str());
  hEff->SetTitle(";Leading JetTagger LRJ E_{T} threshold [GeV]; Signal (hh->4b) Efficiency");
  for (int ib = 1; ib <= nb; ++ib) {
    double eff = (totalSig > 0 ? hSigCum->GetBinContent(ib)/totalSig : 0.0);
    hEff->SetBinContent(ib, std::clamp(eff, 0.0, 1.0));
    hEff->SetBinError  (ib, 0.0); // (optional: compute binomial errors if you want)
  }

  // 3) background rate vs threshold (already in Hz because you weighted on fill)
  auto* hRate = (TH1*)hBkgCum->Clone((std::string(hBkg->GetName())+"_rate_vs_thr").c_str());
  hRate->SetTitle(";Leading JetTagger LRJ E_{T} threshold [GeV];Estimated Background Rate [Hz]");

  // 4) build Rate vs Efficiency curve (one point per threshold bin)
  auto* g = new TGraphErrors(nb);
  g->SetName("gRate_vsEff");
  g->SetTitle("Trigger Rate vs Signal Efficiency; Signal (hh->4b) Efficiency;Estimated Background Rate [Hz]");
  for (int ib = 1; ib <= nb; ++ib) {
    const double eff  = hEff ->GetBinContent(ib);
    const double rate = hRate->GetBinContent(ib);
    double xerr = 0.0;              // no threshold uncertainty here
    double yerr = hRate->GetBinError(ib);  // background stat. error
    g->SetPoint(ib-1, eff, rate);
    g->SetPointError(ib-1, xerr, yerr);
  }

  return {hEff, hRate, g};
}

double FindThrForRate(const TH1* hRateVsThr, double targetHz) {
  const int n = hRateVsThr->GetNbinsX();
  double x0 = hRateVsThr->GetXaxis()->GetBinCenter(1);
  double y0 = hRateVsThr->GetBinContent(1);

  for (int i = 2; i <= n; ++i) {
    double x1 = hRateVsThr->GetXaxis()->GetBinCenter(i);
    double y1 = hRateVsThr->GetBinContent(i);

    // look for a bracket around targetHz (works if rate is monotonic vs threshold)
    const bool brackets = ( (y0 - targetHz)*(y1 - targetHz) <= 0 );
    if (brackets && y1 != y0) {
      const double t = (targetHz - y0) / (y1 - y0);    // linear interpolation fraction
      return x0 + t * (x1 - x0);                       // threshold (GeV)
    }
    x0 = x1; y0 = y1;
  }
  return std::numeric_limits<double>::quiet_NaN();     // not found
}


// ---------- Draw ONE event to a canvas and save ----------
void DrawEventDisplay(const EventDisplayInputs& ev,
                      TCanvas& c,
                      double circleR = 1.0,
                      double labelDy  = 0.6) {
  if (!ev.heatmap) return;

  c.Clear();   // reuse the same canvas safely
  c.cd();

  // Heatmap
  ev.heatmap->GetZaxis()->SetTitle("E_{T} [GeV]");
  ev.heatmap->Draw("COLZ");

  // jFEX seeds: red, dashed
  for (const auto& p : ev.jFexSeeds) {
    TEllipse* e = new TEllipse(p.first, p.second, circleR, circleR);
    e->SetLineColor(kRed);
    e->SetLineWidth(2);
    e->SetFillStyle(0);
    e->SetLineStyle(2);   // dashed
    e->Draw("same");
  }

  // Additional SRJs: azure, dashed
  for (const auto& p : ev.additionalSRJs) {
    TEllipse* e = new TEllipse(p.first, p.second, circleR, circleR);
    e->SetLineColor(kAzure+2);
    e->SetLineWidth(2);
    e->SetFillStyle(0);
    e->SetLineStyle(2);   // dashed
    e->Draw("same");
  }

  // New seeds: green, solid + labels
  const size_t nlab = std::min(ev.newSeeds.size(), ev.newSeedsEt.size());
  for (size_t i = 0; i < ev.newSeeds.size(); ++i) {
    const auto& p = ev.newSeeds[i];
    TEllipse* e = new TEllipse(p.first, p.second, circleR, circleR);
    e->SetLineColor(kGreen+2);
    e->SetLineWidth(2);
    e->SetFillStyle(0);
    e->SetLineStyle(1);   // solid
    e->Draw("same");

    if (i < nlab) {
      TString lab = Form("#eta=%.2f, #phi=%.2f, E_{T}=%.1f GeV", p.first, p.second, ev.newSeedsEt[i]);
      TLatex lat;
      lat.SetTextSize(0.025);
      lat.SetTextColor(kBlack);
      lat.DrawLatex(p.first, p.second + labelDy, lab);
    }
  }

  c.Modified(); c.Update();
  if (!ev.outFile.IsNull()) c.SaveAs(ev.outFile);
}

// ---------- Draw MANY events (loop) ----------
void DrawManyEventDisplays(const std::vector<EventDisplayInputs>& events,
                           TCanvas& c,
                           double circleR = 1.0,
                           double labelDy  = 0.6) {
  for (const auto& ev : events) {
    DrawEventDisplay(ev, c, circleR, labelDy);
  }
}


// ---------- Standalone top ROC (no subplot), same styling, with legend + x-axis ----------
void SaveStandaloneROC(const char* cname,
                       const std::vector<TGraph*>& graphs,
                       TLegend* srcLegend,
                       const TString& outpath,
                       double ymin = 1.0, double ymax = 1e4) {
    if (graphs.empty() || !graphs[0]) return;

    TCanvas* c = new TCanvas(cname, cname, 700, 650);
    c->SetLogy();
    c->SetTicks(1, 1);

    // Margins: give more space for labels/titles
    c->SetLeftMargin(0.16);
    c->SetBottomMargin(0.16);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.05);

    // First graph
    TGraph* g0 = (TGraph*)graphs[0]->Clone(TString(cname) + "_g0");
    g0->SetTitle("ROC Curve;Signal Efficiency;Background Rejection (1/FPR)");
    g0->GetXaxis()->SetRangeUser(0.0, 1.0);
    //g0->GetYaxis()->SetRangeUser(ymin, ymax);

    // Axis label/title sizes
    g0->GetXaxis()->SetLabelSize(0.045);
    g0->GetXaxis()->SetTitleSize(0.055);
    g0->GetYaxis()->SetLabelSize(0.045);
    g0->GetYaxis()->SetTitleSize(0.055);
    g0->GetYaxis()->CenterTitle(true);

    g0->Draw("AL");

    for (size_t i = 1; i < graphs.size(); ++i) {
        if (!graphs[i]) continue;
        TGraph* gi = (TGraph*)graphs[i]->Clone(TString(cname) + Form("_g%zu", i));
        gi->Draw("L SAME");
    }

    // Legend: smaller, moved slightly down/left
    if (srcLegend) {
        TLegend* leg = (TLegend*)srcLegend->Clone(TString(cname) + "_legend");
        leg->SetX1NDC(0.58); leg->SetY1NDC(0.58);
        leg->SetX2NDC(0.88); leg->SetY2NDC(0.83);
        leg->SetTextSize(0.032);
        leg->SetBorderSize(0);   // cleaner look
        leg->SetFillStyle(0);    // transparent
        leg->Draw();
    }

    c->Modified(); c->Update();
    c->SaveAs(outpath);
}

// ------------------------------------------------------------
// Style + draw a stack of 10 histograms onto one canvas and
// save as a single-page PDF.
// ------------------------------------------------------------
void OverlayAndSave(TH1F* hists[], int n, const char* canvasName,
                    TString pdfOut, unsigned int fillStyle,
                    const char* drawOpt = "HIST")
{
    if (n <= 0 || !hists[0]) return;

    // Color palette to mimic your legend image:
    // JZ0 red, JZ1 green, JZ2 blue, JZ3 yellow, JZ4 magenta,
    // JZ5 cyan, JZ6 teal-ish, JZ7 violet, JZ8 light gray, JZ9 dark gray.
    const Color_t colors[10] = {
        kRed+1,      // JZ0
        kGreen+1,    // JZ1
        kBlue,       // JZ2
        kYellow+1,   // JZ3
        kMagenta+1,  // JZ4
        kCyan+1,     // JZ5
        kTeal+2,     // JZ6
        kViolet+1,   // JZ7
        kGray+1,     // JZ8
        kGray+3      // JZ9
    };

    TCanvas* c = new TCanvas(canvasName, canvasName, 900, 700);
    c->SetMargin(0.12, 0.22, 0.16, 0.06); // leave room for right-side legend
    c->SetLogy();

    // Build legend on the right (similar look to your screenshot)
    // (colored boxes, no border, transparent background)
    TLegend* leg = new TLegend(0.80, 0.15, 0.97, 0.92);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.030);

    // Determine a nice max so everything fits
    //double ymax = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!hists[i]) continue;
        // Style
        const Color_t col = colors[i % 10];
        hists[i]->SetLineColor(col);
        hists[i]->SetLineWidth(2);
        hists[i]->SetFillColor(col);
        hists[i]->SetFillStyle(fillStyle); // solid box in legend
        hists[i]->GetXaxis()->SetTitleSize(0.045);
        hists[i]->GetYaxis()->SetTitleSize(0.045);
        hists[i]->GetXaxis()->SetLabelSize(0.040);
        hists[i]->GetYaxis()->SetLabelSize(0.040);
        hists[i]->SetMinimum(1e-7);
        hists[i]->SetMaximum(5e8);

        // Track max
        //if (hists[i]->GetMaximum() > ymax) ymax = hists[i]->GetMaximum();
    }

    // Draw first one to set axes
    //hists[0]->SetMaximum(ymax * 1.20);
    hists[n - 1]->Draw("HIST");

    // Draw the rest
    for (int i = n - 2; i >= 0; i--) {
        if (!hists[i]) continue;
        hists[i]->Draw(TString::Format("HIST SAME"));
    }

    // Add legend entries in order JZ0..JZ9 with colored boxes
    for (int i = 0; i < n; ++i) {
        if (!hists[i]) continue;
        const Color_t col = colors[i % 10];
        leg->AddEntry(hists[i], TString::Format("Slice JZ%d", i), "f")->SetFillColor(col);
    }
    leg->Draw();

    // after drawing all hists
    double x1 = gPad->GetUxmin(), x2 = gPad->GetUxmax();
    double y1 = 1e-7, y2 = 1e-7 * pow(10, 0.03);  // a very thin band above min
    TBox* cover = new TBox(x1, y1, x2, y2);
    cover->SetFillColor(kWhite);
    cover->SetLineColor(kWhite);
    cover->Draw("same");
    gPad->RedrawAxis();

    // Write the PDF
    c->SaveAs(pdfOut);

    // (optional) keep objects alive outside this function if needed
    // delete c; // uncomment if you manage object ownership elsewhere
}

void OverlayAndSaveStack(TH1F* hists[], int n, const char* canvasName,
                         TString pdfOut, unsigned int fillStyle,
                         const char* drawOpt = "HIST")
{
    if (n <= 0 || !hists) return;

    const Color_t colors[10] = { kRed+1, kGreen+1, kBlue, kYellow+1, kMagenta+1,
                                 kCyan+1, kTeal+2, kViolet+1, kGray+1, kGray+3 };

    TCanvas* c = new TCanvas(canvasName, canvasName, 900, 700);
    c->SetMargin(0.12, 0.22, 0.16, 0.06);
    c->SetLogy();

    TLegend* leg = new TLegend(0.80, 0.15, 0.97, 0.92);
    leg->SetBorderSize(0); leg->SetFillStyle(0); leg->SetTextSize(0.030);

    // Don't pass " ; ; " – it clears axis titles.
    THStack* hs = new THStack(TString::Format("%s_stack", canvasName), canvasName);

    int firstIdx = -1;
    for (int i = 0; i < n; ++i) {
        if (!hists[i]) continue;
        if (firstIdx < 0) firstIdx = i;

        const Color_t col = colors[i % 10];
        hists[i]->SetLineColor(col);
        hists[i]->SetLineWidth(2);
        hists[i]->SetFillColor(col);
        hists[i]->SetFillStyle(fillStyle);

        hs->Add(hists[i], drawOpt);
        leg->AddEntry(hists[i], TString::Format("Slice JZ%d", i), "f");
    }
    if (firstIdx < 0) return;

    // Draw once to create the internal histogram/axes
    hs->Draw(drawOpt);
    gPad->Update();

    // --- Set axis titles on the STACK (not on each TH1) ---
    if (hs->GetXaxis()) hs->GetXaxis()->SetTitle(hists[firstIdx]->GetXaxis()->GetTitle());
    if (hs->GetYaxis()) hs->GetYaxis()->SetTitle(hists[firstIdx]->GetYaxis()->GetTitle());
    if (hs->GetXaxis()) { hs->GetXaxis()->SetTitleSize(0.045); hs->GetXaxis()->SetLabelSize(0.040); }
    if (hs->GetYaxis()) { hs->GetYaxis()->SetTitleSize(0.045); hs->GetYaxis()->SetLabelSize(0.040); }

    // --- Control y-range on the STACK (not the components) ---
    const double ymin = 1e-7;
    hs->SetMinimum(ymin);                 // this is the one that matters for THStack
    const double ymax = hs->GetMaximum(); // after Draw()
    hs->SetMaximum((ymax > ymin ? ymax : ymin*10.0) * 1.20);

    leg->Draw();
    gPad->Modified();                     // make ROOT re-compute the pad
    gPad->Update();

    c->SaveAs(pdfOut);
}


struct GlobalRateEffOut {
  TGraphErrors* gRate_vsEff_combined; // frontier only (best eff per rate)

  // total (all categories combined)
  double bestEff;   // global total efficiency
  double bestRate;  // global total rate

  // thresholds at best point
  double bestEtCut_0;
  double bestEtCut_1;
  double bestEtCut_2;
  double bestMassCut_2;

  // per-category conditional efficiencies at best point
  // (efficiency within each subjet category)
  double bestEff0_cat;
  double bestEff1_cat;
  double bestEff2_cat;

  // per-category *global* contributions to signal efficiency
  // (these sum to bestEff)
  double bestEff0_tot;
  double bestEff1_tot;
  double bestEff2_tot;

  // per-category rates
  double bestRate0;
  double bestRate1;
  double bestRate2;
};



GlobalRateEffOut MakeCombinedRateVsEff_AllSubjets(
    const RateEffOut&   out0,
    const RateEffOut&   out1,
    const RateEff2DOut& out2D,
    TH1* sig0,        // signal hist for 0 subjets
    TH1* sig1,        // signal hist for 1 subjet
    TH2* sig2D,       // signal hist for >=2 subjets
    double maxRateHz = 1.0e4) // e.g. 10 kHz constraint
{
  if (!sig0 || !sig1 || !sig2D) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllSubjets: null signal hist");
  }

  const int nb0  = out0.hEff_vsThr->GetNbinsX();
  const int nb1  = out1.hEff_vsThr->GetNbinsX();
  const int nb2x = out2D.hEff_vsThr_vsR->GetNbinsX();
  const int nb2y = out2D.hEff_vsThr_vsR->GetNbinsY();

  const double nSig0 = sig0->Integral(1, nb0);
  const double nSig1 = sig1->Integral(1, nb1);
  const double nSig2 = sig2D->Integral(1, nb2x, 1, nb2y);
  const double totalSig = nSig0 + nSig1 + nSig2;

  std::cout << "nSig0,1,2: " << nSig0 << " , " << nSig1 << " , " << nSig2 << "\n";
  std::cout << "totalSig: " << totalSig << "\n";
  if (totalSig != 10000) {
    std::cout << "ERROR: not all signal included in histograms scanned over!" << "\n";
  }

  if (totalSig <= 0.0) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllSubjets: totalSig <= 0");
  }

  const double frac0 = nSig0 / totalSig;
  const double frac1 = nSig1 / totalSig;
  const double frac2 = nSig2 / totalSig;

  struct GlobalPoint {
    // global total eff
    double eff;

    // per-category conditional effs (within each category)
    double eff0_cat;
    double eff1_cat;
    double eff2_cat;

    // per-category global contributions (eff_cat * frac_cat)
    double eff0_tot;
    double eff1_tot;
    double eff2_tot;

    // rates
    double rate;    // total
    double rate0;
    double rate1;
    double rate2;

    double rateErr; // total stat error

    // thresholds
    double etCut0;
    double etCut1;
    double etCut2;
    double massCut2;
  };

  std::vector<GlobalPoint> allPoints;
  allPoints.reserve(nb0 * nb1 * nb2x * nb2y);

  // ---- (1) Enumerate all combinations, apply rate cut, store raw points ----
  for (int i0 = 1; i0 <= nb0; ++i0) {
    const double eff0_cat = out0.hEff_vsThr->GetBinContent(i0); // conditional eff in cat 0
    const double rate0    = out0.hRate_vsThr->GetBinContent(i0);
    const double err0     = out0.hRate_vsThr->GetBinError(i0);
    const double etCut0   = out0.hEff_vsThr->GetXaxis()->GetBinLowEdge(i0);

    for (int i1 = 1; i1 <= nb1; ++i1) {
      const double eff1_cat = out1.hEff_vsThr->GetBinContent(i1);
      const double rate1    = out1.hRate_vsThr->GetBinContent(i1);
      const double err1     = out1.hRate_vsThr->GetBinError(i1);
      const double etCut1   = out1.hEff_vsThr->GetXaxis()->GetBinLowEdge(i1);

      for (int ix2 = 1; ix2 <= nb2x; ++ix2) {
        for (int iy2 = 1; iy2 <= nb2y; ++iy2) {
          const double eff2_cat = out2D.hEff_vsThr_vsR->GetBinContent(ix2, iy2);
          const double rate2    = out2D.hRate_vsThr_vsR->GetBinContent(ix2, iy2);
          const double err2     = out2D.hRate_vsThr_vsR->GetBinError(ix2, iy2);
          const double etCut2   = out2D.hEff_vsThr_vsR->GetXaxis()->GetBinLowEdge(ix2);
          const double massCut2 = out2D.hEff_vsThr_vsR->GetYaxis()->GetBinLowEdge(iy2);

          // per-category global contributions
          const double eff0_tot = eff0_cat * frac0;
          const double eff1_tot = eff1_cat * frac1;
          const double eff2_tot = eff2_cat * frac2;
          const double effTot   = eff0_tot + eff1_tot + eff2_tot;

          const double rateTot = rate0 + rate1 + rate2;
          const double errTot  = std::sqrt(err0*err0 + err1*err1 + err2*err2);

          if (rateTot > maxRateHz) continue;

          GlobalPoint gp;
          gp.eff      = effTot;

          gp.eff0_cat = eff0_cat;
          gp.eff1_cat = eff1_cat;
          gp.eff2_cat = eff2_cat;

          gp.eff0_tot = eff0_tot;
          gp.eff1_tot = eff1_tot;
          gp.eff2_tot = eff2_tot;

          gp.rate     = rateTot;
          gp.rate0    = rate0;
          gp.rate1    = rate1;
          gp.rate2    = rate2;

          gp.rateErr  = errTot;
          gp.etCut0   = etCut0;
          gp.etCut1   = etCut1;
          gp.etCut2   = etCut2;
          gp.massCut2 = massCut2;

          allPoints.push_back(gp);
        }
      }
    }
  }

  // ---- (2) Build "best efficiency per rate" frontier ----------------------
  std::sort(allPoints.begin(), allPoints.end(),
            [](const GlobalPoint& a, const GlobalPoint& b) {
              if (a.rate < b.rate) return true;
              if (a.rate > b.rate) return false;
              return a.eff > b.eff; // for same rate, keep higher eff first
            });

  std::vector<GlobalPoint> frontier;
  frontier.reserve(allPoints.size());

  double bestEffSoFar = -1.0;
  const double eps = 1e-8;

  for (const auto& gp : allPoints) {
    if (gp.eff > bestEffSoFar + eps) {
      frontier.push_back(gp);
      bestEffSoFar = gp.eff;
    }
  }

  // ---- (3) Build TGraphErrors only from frontier points & pick best -------

  auto* gCombined = new TGraphErrors(frontier.size());
  gCombined->SetName("gRate_vsEff_all_frontier");
  gCombined->SetTitle("Combined Trigger Rate vs Signal Efficiency;"
                      "Signal (hh#rightarrow4b) Efficiency (all categories combined);"
                      "Total Estimated Background Rate [Hz]");

  double bestEff   = -1.0;
  double bestRate  =  0.0;
  double bestEt0   =  0.0;
  double bestEt1   =  0.0;
  double bestEt2   =  0.0;
  double bestMass2 =  0.0;

  double bestEff0_cat = 0.0;
  double bestEff1_cat = 0.0;
  double bestEff2_cat = 0.0;

  double bestEff0_tot = 0.0;
  double bestEff1_tot = 0.0;
  double bestEff2_tot = 0.0;

  double bestRate0 = 0.0;
  double bestRate1 = 0.0;
  double bestRate2 = 0.0;

  for (size_t i = 0; i < frontier.size(); ++i) {
    const auto& gp = frontier[i];
    gCombined->SetPoint(i, gp.eff, gp.rate);
    gCombined->SetPointError(i, 0.0, gp.rateErr);

    if (gp.eff > bestEff) {
      bestEff   = gp.eff;
      bestRate  = gp.rate;
      bestEt0   = gp.etCut0;
      bestEt1   = gp.etCut1;
      bestEt2   = gp.etCut2;
      bestMass2 = gp.massCut2;

      bestEff0_cat = gp.eff0_cat;
      bestEff1_cat = gp.eff1_cat;
      bestEff2_cat = gp.eff2_cat;

      bestEff0_tot = gp.eff0_tot;
      bestEff1_tot = gp.eff1_tot;
      bestEff2_tot = gp.eff2_tot;

      bestRate0 = gp.rate0;
      bestRate1 = gp.rate1;
      bestRate2 = gp.rate2;
    }
  }

  GlobalRateEffOut out;
  out.gRate_vsEff_combined = gCombined;

  out.bestEff   = bestEff;
  out.bestRate  = bestRate;

  out.bestEtCut_0   = bestEt0;
  out.bestEtCut_1   = bestEt1;
  out.bestEtCut_2   = bestEt2;
  out.bestMassCut_2 = bestMass2;

  out.bestEff0_cat = bestEff0_cat;
  out.bestEff1_cat = bestEff1_cat;
  out.bestEff2_cat = bestEff2_cat;

  out.bestEff0_tot = bestEff0_tot;
  out.bestEff1_tot = bestEff1_tot;
  out.bestEff2_tot = bestEff2_tot;

  out.bestRate0 = bestRate0;
  out.bestRate1 = bestRate1;
  out.bestRate2 = bestRate2;

  return out;
}



struct Cat1DPoint {
  double eff_cat;   // efficiency within this category
  double rate_cat;  // rate in Hz
  double err_cat;   // stat error on rate
  double thrEt;     // Et threshold
};

struct Cat2DPoint {
  double eff_cat;
  double rate_cat;
  double err_cat;
  double thrEt;     // Et threshold
  double thrMass;   // mass threshold
};

struct Cat3DPoint {
  double eff_cat;
  double rate_cat;
  double err_cat;
  double thrEtSub;      // subleading Et threshold
  double thrMassLead;   // leading mass threshold
  double thrMassSub;    // subleading mass threshold
};

// Build frontier for 1D rate-vs-eff (RateEffOut) selection
static std::vector<Cat1DPoint>
BuildFrontier1D(const RateEffOut& out, double maxRateHz)
{
  const int nb = out.hEff_vsThr->GetNbinsX();
  std::vector<Cat1DPoint> pts;
  pts.reserve(nb);

  for (int i = 1; i <= nb; ++i) {
    const double eff  = out.hEff_vsThr ->GetBinContent(i);
    const double rate = out.hRate_vsThr->GetBinContent(i);
    const double err  = out.hRate_vsThr->GetBinError(i);
    if (rate > maxRateHz) continue;  // hopeless on its own

    const double thrEt = out.hEff_vsThr->GetXaxis()->GetBinLowEdge(i);
    pts.push_back({eff, rate, err, thrEt});
  }

  // Sort by rate ascending, and for equal rate, by efficiency descending
  std::sort(pts.begin(), pts.end(),
            [](const Cat1DPoint& a, const Cat1DPoint& b) {
              if (a.rate_cat < b.rate_cat) return true;
              if (a.rate_cat > b.rate_cat) return false;
              return a.eff_cat > b.eff_cat;
            });

  // Keep only frontier (monotonic in efficiency)
  std::vector<Cat1DPoint> frontier;
  frontier.reserve(pts.size());
  double bestEffSoFar = -1.0;
  const double eps = 1e-8;

  for (const auto& p : pts) {
    if (p.eff_cat > bestEffSoFar + eps) {
      frontier.push_back(p);
      bestEffSoFar = p.eff_cat;
    }
  }
  return frontier;
}

// Build frontier for 2D (Et,Mass) selection
static std::vector<Cat2DPoint>
BuildFrontier2D(const RateEff2DOut& out2D, double maxRateHz)
{
  TH2* hEff  = out2D.hEff_vsThr_vsR;
  TH2* hRate = out2D.hRate_vsThr_vsR;

  const int nbx = hEff->GetNbinsX();
  const int nby = hEff->GetNbinsY();

  std::vector<Cat2DPoint> pts;
  pts.reserve(nbx * nby);

  for (int ix = 1; ix <= nbx; ++ix) {
    for (int iy = 1; iy <= nby; ++iy) {
      const double eff  = hEff ->GetBinContent(ix, iy);
      const double rate = hRate->GetBinContent(ix, iy);
      const double err  = hRate->GetBinError  (ix, iy);
      if (rate > maxRateHz) continue;

      const double thrEt   = hEff->GetXaxis()->GetBinLowEdge(ix);
      const double thrMass = hEff->GetYaxis()->GetBinLowEdge(iy);

      pts.push_back({eff, rate, err, thrEt, thrMass});
    }
  }

  std::sort(pts.begin(), pts.end(),
            [](const Cat2DPoint& a, const Cat2DPoint& b) {
              if (a.rate_cat < b.rate_cat) return true;
              if (a.rate_cat > b.rate_cat) return false;
              return a.eff_cat > b.eff_cat;
            });

  std::vector<Cat2DPoint> frontier;
  frontier.reserve(pts.size());
  double bestEffSoFar = -1.0;
  const double eps = 1e-8;

  for (const auto& p : pts) {
    if (p.eff_cat > bestEffSoFar + eps) {
      frontier.push_back(p);
      bestEffSoFar = p.eff_cat;
    }
  }
  return frontier;
}

// Build frontier for 3D (Et_sub, m_lead, m_sub) selection
static std::vector<Cat3DPoint>
BuildFrontier3D(const RateEff3DOut& out3D, double maxRateHz)
{
  TH3* hEff  = out3D.hEff_vsThr3D;
  TH3* hRate = out3D.hRate_vsThr3D;

  const int nbx = hEff->GetNbinsX();
  const int nby = hEff->GetNbinsY();
  const int nbz = hEff->GetNbinsZ();

  std::vector<Cat3DPoint> pts;
  pts.reserve(nbx * nby * nbz);

  for (int ix = 1; ix <= nbx; ++ix) {
    for (int iy = 1; iy <= nby; ++iy) {
      for (int iz = 1; iz <= nbz; ++iz) {
        const double eff  = hEff ->GetBinContent(ix, iy, iz);
        const double rate = hRate->GetBinContent(ix, iy, iz);
        const double err  = hRate->GetBinError  (ix, iy, iz);
        if (rate > maxRateHz) continue;

        const double thrEtSub    = hEff->GetXaxis()->GetBinLowEdge(ix);
        const double thrMassLead = hEff->GetYaxis()->GetBinLowEdge(iy);
        const double thrMassSub  = hEff->GetZaxis()->GetBinLowEdge(iz);

        pts.push_back({eff, rate, err, thrEtSub, thrMassLead, thrMassSub});
      }
    }
  }

  std::sort(pts.begin(), pts.end(),
            [](const Cat3DPoint& a, const Cat3DPoint& b) {
              if (a.rate_cat < b.rate_cat) return true;
              if (a.rate_cat > b.rate_cat) return false;
              return a.eff_cat > b.eff_cat;
            });

  std::vector<Cat3DPoint> frontier;
  frontier.reserve(pts.size());
  double bestEffSoFar = -1.0;
  const double eps = 1e-8;

  for (const auto& p : pts) {
    if (p.eff_cat > bestEffSoFar + eps) {
      frontier.push_back(p);
      bestEffSoFar = p.eff_cat;
    }
  }
  return frontier;
}



struct GlobalRateEffOut5 {
  TGraphErrors* gRate_vsEff_combined; // frontier only (best eff per rate)

  // total (all 5 categories combined)
  double bestEff;   // global total efficiency
  double bestRate;  // global total rate

  // thresholds at best point
  //  (0) 0/1-subjet selection, leading Et scan
  double bestEtCut_0_lead;
  //  (1) 0/1-subjet selection, subleading Et scan
  double bestEtCut_0_sub;
  //  (2) >=2 lead subjets, leading Et vs mass
  double bestEtCut_lead2;
  double bestMassCut_lead2;
  //  (3) >=2 subleading subjets, subleading Et vs mass
  double bestEtCut_sub2;
  double bestMassCut_sub2;
  //  (4) >=2 lead & >=2 subleading subjets, 3D Et_sub vs m_lead vs m_sub
  double bestEtCut_3D_sub;
  double bestMassCut_3D_lead;
  double bestMassCut_3D_sub;

  // per-category conditional efficiencies (within each category)
  double bestEff_cat[5]; // eff within that category only

  // per-category global contributions to efficiency (sum to bestEff)
  double bestEff_tot[5]; // eff_cat * frac_cat

  // fraction of total signal events in each category (sum to 1)
  double fractionEventsPerCat[5];

  // per-category rates
  double bestRate_cat[5]; // Hz
};

// Helper structs & functions for the 5-category combination
namespace {

  struct GlobalPoint {
    double eff;            // global total efficiency

    double eff_cat[5];     // per-category conditional efficiencies
    double eff_tot[5];     // per-category global contributions

    double rate;           // total rate
    double rate_cat[5];    // per-category rates

    double rateErr;        // total stat error on rate

    // thresholds
    double etCut0_lead;
    double etCut0_sub;

    double etCut_lead2;
    double massCut_lead2;

    double etCut_sub2;
    double massCut_sub2;

    double etCut_3D_sub;
    double massCut_3D_lead;
    double massCut_3D_sub;
  };

  // Maintain a global (rate,eff) frontier incrementally
  void UpdateGlobalFrontier(std::vector<GlobalPoint>& frontier,
                            const GlobalPoint& gp)
  {
    const double eps = 1e-8;

    // 1) If gp is dominated by an existing frontier point, drop it.
    // Dominated means: existing has <= rate and >= eff.
    for (const auto& f : frontier) {
      if (f.rate <= gp.rate + eps && f.eff >= gp.eff - eps) {
        return;  // gp adds nothing new
      }
    }

    // 2) Remove any frontier points dominated by gp:
    // gp has <= rate and >= eff.
    frontier.erase(
      std::remove_if(frontier.begin(), frontier.end(),
        [&](const GlobalPoint& f){
          return (gp.rate <= f.rate + eps && gp.eff >= f.eff - eps);
        }),
      frontier.end()
    );

    // 3) Insert gp in sorted order by rate (then eff desc).
    auto it = std::upper_bound(
        frontier.begin(), frontier.end(), gp,
        [](const GlobalPoint& a, const GlobalPoint& b) {
          if (a.rate < b.rate) return true;
          if (a.rate > b.rate) return false;
          return a.eff > b.eff;
        }
    );
    frontier.insert(it, gp);
  }

  // Generic helper: keep only frontier points with rate in [minRate, maxRate]
  template <typename T>
  std::vector<T> FilterFrontierByRate(const std::vector<T>& in,
                                      double                minRate,
                                      double                maxRate)
  {
    std::vector<T> out;
    out.reserve(in.size());
    for (const auto& p : in) {
      if (p.rate_cat >= minRate && p.rate_cat <= maxRate) {
        out.push_back(p);
      }
    }
    return out;
  }

} // anonymous namespace


// Designed for 5 separate selections depending on the number of subjets
GlobalRateEffOut5 MakeCombinedRateVsEff_AllFive(
    const RateEffOut&   out0_leadEt,   // 1D: leading Et (0/1 subjets)
    const RateEffOut&   out0_subEt,    // 1D: subleading Et (0/1 subjets)
    const RateEff2DOut& out2D_lead2,   // 2D: leading Et vs mass (>=2 lead subjets)
    const RateEff2DOut& out2D_sub2,    // 2D: subleading Et vs mass (>=2 subl subjets)
    const RateEff3DOut& out3D_both2,   // 3D: subleading Et vs m_lead vs m_sub (>=2 lead & subl)
    TH1* sig0_leadEt,                  // signal hist for 1D leading Et
    TH1* sig0_subEt,                   // signal hist for 1D subleading Et
    TH2* sig2D_lead2,                  // signal hist for 2D leading (Et,mass)
    TH2* sig2D_sub2,                   // signal hist for 2D subleading (Et,mass)
    TH3* sig3D_both2,                  // signal hist for 3D (Et_sub,m_lead,m_sub)
    double maxRateHz,                  // e.g. 10 kHz constraint
    double maxRateHzPrint                )
{
  // Only consider per-category frontier points with rate in [minRateHz, maxRateHz]
  const double minRateHz = 10.0;

  // Basic null checks
  if (!sig0_leadEt || !sig0_subEt || !sig2D_lead2 || !sig2D_sub2 || !sig3D_both2) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: null signal hist");
  }

  if (!out0_leadEt.hEff_vsThr || !out0_leadEt.hRate_vsThr ||
      !out0_subEt .hEff_vsThr || !out0_subEt .hRate_vsThr ||
      !out2D_lead2.hEff_vsThr_vsR || !out2D_lead2.hRate_vsThr_vsR ||
      !out2D_sub2 .hEff_vsThr_vsR || !out2D_sub2 .hRate_vsThr_vsR ||
      !out3D_both2.hEff_vsThr3D  || !out3D_both2.hRate_vsThr3D) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: one or more RateEff*Out hists are null");
  }

  // --- (0) Signal fractions per category -----------------------------------
  const int nb0L  = out0_leadEt.hEff_vsThr->GetNbinsX();
  const int nb0S  = out0_subEt .hEff_vsThr->GetNbinsX();
  const int nb2Lx = out2D_lead2.hEff_vsThr_vsR->GetNbinsX();
  const int nb2Ly = out2D_lead2.hEff_vsThr_vsR->GetNbinsY();
  const int nb2Sx = out2D_sub2 .hEff_vsThr_vsR->GetNbinsX();
  const int nb2Sy = out2D_sub2 .hEff_vsThr_vsR->GetNbinsY();
  const int nb3X  = out3D_both2.hEff_vsThr3D->GetNbinsX();
  const int nb3Y  = out3D_both2.hEff_vsThr3D->GetNbinsY();
  const int nb3Z  = out3D_both2.hEff_vsThr3D->GetNbinsZ();

  const double nSig0L = sig0_leadEt->Integral(1, nb0L);
  const double nSig0S = sig0_subEt ->Integral(1, nb0S);
  const double nSig2L = sig2D_lead2->Integral(1, nb2Lx, 1, nb2Ly);
  const double nSig2S = sig2D_sub2 ->Integral(1, nb2Sx, 1, nb2Sy);
  const double nSig3D = sig3D_both2->Integral(1, nb3X, 1, nb3Y, 1, nb3Z);

  const double totalSig = nSig0L + nSig0S + nSig2L + nSig2S + nSig3D;

  std::cout << "[MakeCombinedRateVsEff_AllFive] nSig0L,0S,2L,2S,3D: "
            << nSig0L << " , " << nSig0S << " , "
            << nSig2L << " , " << nSig2S << " , "
            << nSig3D << "\n";
  std::cout << "[MakeCombinedRateVsEff_AllFive] totalSig: " << totalSig << "\n";

  if (totalSig <= 0.0) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: totalSig <= 0");
  }

  const double frac0L = nSig0L / totalSig;
  const double frac0S = nSig0S / totalSig;
  const double frac2L = nSig2L / totalSig;
  const double frac2S = nSig2S / totalSig;
  const double frac3D = nSig3D / totalSig;

  // --- (1) Build per-category frontiers -----------------------------------
  auto front0L_raw = BuildFrontier1D(out0_leadEt, maxRateHz);
  auto front0S_raw = BuildFrontier1D(out0_subEt , maxRateHz);
  auto front2L_raw = BuildFrontier2D(out2D_lead2, maxRateHz);
  auto front2S_raw = BuildFrontier2D(out2D_sub2 , maxRateHz);
  auto front3D_raw = BuildFrontier3D(out3D_both2, maxRateHz);

  // Further restrict to per-category rate in [minRateHz, maxRateHz]
  auto front0L = FilterFrontierByRate(front0L_raw, minRateHz, maxRateHz);
  auto front0S = FilterFrontierByRate(front0S_raw, minRateHz, maxRateHz);
  auto front2L = FilterFrontierByRate(front2L_raw, minRateHz, maxRateHz);
  auto front2S = FilterFrontierByRate(front2S_raw, minRateHz, maxRateHz);
  auto front3D = FilterFrontierByRate(front3D_raw, minRateHz, maxRateHz);

  std::cout << "[MakeCombinedRateVsEff_AllFive] raw frontier sizes: "
            << "0L=" << front0L_raw.size()
            << ", 0S=" << front0S_raw.size()
            << ", 2L=" << front2L_raw.size()
            << ", 2S=" << front2S_raw.size()
            << ", 3D=" << front3D_raw.size() << "\n";

  std::cout << "[MakeCombinedRateVsEff_AllFive] filtered ("
            << minRateHz << "–" << maxRateHz << " Hz) frontier sizes: "
            << "0L=" << front0L.size()
            << ", 0S=" << front0S.size()
            << ", 2L=" << front2L.size()
            << ", 2S=" << front2S.size()
            << ", 3D=" << front3D.size() << "\n";

    // -------------------------------------------------------------------------
    // OPTION: completely disable the (1) 0/1-subjets subleading Et category
    // in the *numerator* (rate/eff contributions), but KEEP its signal events
    // in the denominator via totalSig (i.e. keep frac0S as computed above).
    // -------------------------------------------------------------------------
    const bool disableCat1_subEt = true;

    if (disableCat1_subEt) {
      std::cerr << "[MakeCombinedRateVsEff_AllFive] INFO: disabling category (1) out0_subEt "
                << "in combination (eff/rate set to 0), but keeping its signal fraction in denominator.\n";

      // Force the category-1 frontier to a single dummy point (no contribution).
      front0S.clear();
      front0S.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
    }

  // If a category has *no* usable points after filtering, treat it as "not used":
  // give it a single dummy point with eff=0, rate=0 so it contributes nothing.
  if (front0L.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 0L frontier empty, using dummy point (no contribution).\n";
    front0L.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
  }
  if (front0S.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 0S frontier empty, using dummy point (no contribution).\n";
    front0S.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
  }
  if (front2L.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 2L frontier empty, using dummy point (no contribution).\n";
    front2L.push_back(Cat2DPoint{0.0, 0.0, 0.0, 0.0, 0.0});
  }
  if (front2S.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 2S frontier empty, using dummy point (no contribution).\n";
    front2S.push_back(Cat2DPoint{0.0, 0.0, 0.0, 0.0, 0.0});
  }
  if (front3D.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 3D frontier empty, using dummy point (no contribution).\n";
    front3D.push_back(Cat3DPoint{0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
  }

  // --- (2) Enumerate combinations over *filtered frontier* points only -----
  std::vector<GlobalPoint> globalFrontier;

  // We assume each frontier is sorted by rate ascending (as in BuildFrontier*).
  for (const auto& p0L : front0L) {
    const double eff0L_cat = p0L.eff_cat;
    const double rate0L    = p0L.rate_cat;
    const double err0L     = p0L.err_cat;
    const double etCut0L   = p0L.thrEt;

    for (const auto& p0S : front0S) {
      const double eff0S_cat = p0S.eff_cat;
      const double rate0S    = p0S.rate_cat;
      const double err0S     = p0S.err_cat;
      const double etCut0S   = p0S.thrEt;

      const double rate01 = rate0L + rate0S;
      if (rate01 > maxRateHz) break; // front0S sorted by rate

      for (const auto& p2L : front2L) {
        const double eff2L_cat = p2L.eff_cat;
        const double rate2L    = p2L.rate_cat;
        const double err2L     = p2L.err_cat;
        const double etCut2L   = p2L.thrEt;
        const double massCut2L = p2L.thrMass;

        const double rate012 = rate01 + rate2L;
        if (rate012 > maxRateHz) break; // front2L sorted by rate

        for (const auto& p2S : front2S) {
          const double eff2S_cat = p2S.eff_cat;
          const double rate2S    = p2S.rate_cat;
          const double err2S     = p2S.err_cat;
          const double etCut2S   = p2S.thrEt;
          const double massCut2S = p2S.thrMass;

          const double rate0123 = rate012 + rate2S;
          if (rate0123 > maxRateHz) break; // front2S sorted by rate

          for (const auto& p3D : front3D) {
            const double eff3D_cat = p3D.eff_cat;
            const double rate3D    = p3D.rate_cat;
            const double err3D     = p3D.err_cat;
            const double etCut3D_sub    = p3D.thrEtSub;
            const double massCut3D_lead = p3D.thrMassLead;
            const double massCut3D_sub  = p3D.thrMassSub;

            const double rateTot = rate0123 + rate3D;
            if (rateTot > maxRateHz) break; // front3D sorted by rate

            // per-category global contributions
            const double eff0L_tot = eff0L_cat * frac0L;
            const double eff0S_tot = eff0S_cat * frac0S;
            const double eff2L_tot = eff2L_cat * frac2L;
            const double eff2S_tot = eff2S_cat * frac2S;
            const double eff3D_tot = eff3D_cat * frac3D;

            const double effTot =
                eff0L_tot + eff0S_tot + eff2L_tot + eff2S_tot + eff3D_tot;

            const double errTot =
                std::sqrt(err0L*err0L + err0S*err0S +
                          err2L*err2L + err2S*err2S + err3D*err3D);

            GlobalPoint gp;
            gp.eff = effTot;

            gp.eff_cat[0] = eff0L_cat;
            gp.eff_cat[1] = eff0S_cat;
            gp.eff_cat[2] = eff2L_cat;
            gp.eff_cat[3] = eff2S_cat;
            gp.eff_cat[4] = eff3D_cat;

            gp.eff_tot[0] = eff0L_tot;
            gp.eff_tot[1] = eff0S_tot;
            gp.eff_tot[2] = eff2L_tot;
            gp.eff_tot[3] = eff2S_tot;
            gp.eff_tot[4] = eff3D_tot;

            gp.rate        = rateTot;
            gp.rate_cat[0] = rate0L;
            gp.rate_cat[1] = rate0S;
            gp.rate_cat[2] = rate2L;
            gp.rate_cat[3] = rate2S;
            gp.rate_cat[4] = rate3D;

            gp.rateErr = errTot;

            gp.etCut0_lead      = etCut0L;
            gp.etCut0_sub       = etCut0S;
            gp.etCut_lead2      = etCut2L;
            gp.massCut_lead2    = massCut2L;
            gp.etCut_sub2       = etCut2S;
            gp.massCut_sub2     = massCut2S;
            gp.etCut_3D_sub     = etCut3D_sub;
            gp.massCut_3D_lead  = massCut3D_lead;
            gp.massCut_3D_sub   = massCut3D_sub;

            UpdateGlobalFrontier(globalFrontier, gp);
          }
        }
      }
    }
  }

  std::cout << "[MakeCombinedRateVsEff_AllFive] global frontier size: "
            << globalFrontier.size() << "\n";

  // --- (3) Build TGraphErrors from globalFrontier & pick best point -------
  auto* gCombined = new TGraphErrors(globalFrontier.size());
  gCombined->SetName("gRate_vsEff_all5_frontier");
  gCombined->SetTitle("Combined Trigger Rate vs Signal Efficiency;"
                      "Signal (hh#rightarrow4b) Efficiency (all 5 selections);"
                      "Total Estimated Background Rate [Hz]");

  GlobalRateEffOut5 out{};
  out.gRate_vsEff_combined = gCombined;

  // store signal fractions for legend
  out.fractionEventsPerCat[0] = frac0L;
  out.fractionEventsPerCat[1] = frac0S;
  out.fractionEventsPerCat[2] = frac2L;
  out.fractionEventsPerCat[3] = frac2S;
  out.fractionEventsPerCat[4] = frac3D;

  out.bestEff  = 0.0;  // default: no selection → eff=0
  out.bestRate = 0.0;  // default: rate=0

  for (size_t i = 0; i < globalFrontier.size(); ++i) {
    const auto& gp = globalFrontier[i];
    gCombined->SetPoint(i, gp.eff, gp.rate);
    gCombined->SetPointError(i, 0.0, gp.rateErr);

    if(gp.rate > maxRateHzPrint) continue; // ensure that the printed values are for a rate of the print value (10kHz)

    if (gp.eff > out.bestEff) {
      out.bestEff = gp.eff;
      out.bestRate = gp.rate;

      out.bestEtCut_0_lead    = gp.etCut0_lead;
      out.bestEtCut_0_sub     = gp.etCut0_sub;
      out.bestEtCut_lead2     = gp.etCut_lead2;
      out.bestMassCut_lead2   = gp.massCut_lead2;
      out.bestEtCut_sub2      = gp.etCut_sub2;
      out.bestMassCut_sub2    = gp.massCut_sub2;
      out.bestEtCut_3D_sub    = gp.etCut_3D_sub;
      out.bestMassCut_3D_lead = gp.massCut_3D_lead;
      out.bestMassCut_3D_sub  = gp.massCut_3D_sub;

      for (int c = 0; c < 5; ++c) {
        out.bestEff_cat[c]  = gp.eff_cat[c];
        out.bestEff_tot[c]  = gp.eff_tot[c];
        out.bestRate_cat[c] = gp.rate_cat[c];
      }
    }
  }

  return out;
}

// Designed for 5 separate selections depending on the number of subjets
GlobalRateEffOut5 MakeCombinedRateVsEff_AllFive_NSubjetiness(
    const RateEffOut&   out0_leadEt,   // 1D: leading Et (0/1 subjets)
    const RateEffOut&   out0_subEt,    // 1D: subleading Et (0/1 subjets)
    const RateEff2DOut& out2D_lead2,   // 2D: leading Et vs mass (>=2 lead subjets)
    const RateEff2DOut& out2D_sub2,    // 2D: subleading Et vs mass (>=2 subl subjets)
    const RateEff2DOut& out2D_both2,   // 3D: subleading Et vs m_lead vs m_sub (>=2 lead & subl)
    TH1* sig0_leadEt,                  // signal hist for 1D leading Et
    TH1* sig0_subEt,                   // signal hist for 1D subleading Et
    TH2* sig2D_lead2,                  // signal hist for 2D leading (Et,mass)
    TH2* sig2D_sub2,                   // signal hist for 2D subleading (Et,mass)
    TH2* sig2D_both2,                  // signal hist for 3D (Et_sub,m_lead,m_sub)
    double maxRateHz,                  // e.g. 10 kHz constraint
    double maxRateHzPrint                )
{
  // Only consider per-category frontier points with rate in [minRateHz, maxRateHz]
  const double minRateHz = 500.0;

  // Basic null checks
  if (!sig0_leadEt || !sig0_subEt || !sig2D_lead2 || !sig2D_sub2 || !sig2D_both2) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: null signal hist");
  }

  if (!out0_leadEt.hEff_vsThr || !out0_leadEt.hRate_vsThr ||
      !out0_subEt .hEff_vsThr || !out0_subEt .hRate_vsThr ||
      !out2D_lead2.hEff_vsThr_vsR || !out2D_lead2.hRate_vsThr_vsR ||
      !out2D_sub2 .hEff_vsThr_vsR || !out2D_sub2 .hRate_vsThr_vsR ||
      !out2D_both2.hEff_vsThr_vsR  || !out2D_both2.hRate_vsThr_vsR) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: one or more RateEff*Out hists are null");
  }

  // --- (0) Signal fractions per category -----------------------------------
  const int nb0L  = out0_leadEt.hEff_vsThr->GetNbinsX();
  const int nb0S  = out0_subEt .hEff_vsThr->GetNbinsX();
  const int nb2Lx = out2D_lead2.hEff_vsThr_vsR->GetNbinsX();
  const int nb2Ly = out2D_lead2.hEff_vsThr_vsR->GetNbinsY();
  const int nb2Sx = out2D_sub2 .hEff_vsThr_vsR->GetNbinsX();
  const int nb2Sy = out2D_sub2 .hEff_vsThr_vsR->GetNbinsY();
  const int nb2BX  = out2D_both2.hEff_vsThr_vsR->GetNbinsX();
  const int nb2BY  = out2D_both2.hEff_vsThr_vsR->GetNbinsY();


  const double nSig0L = sig0_leadEt->Integral(1, nb0L);
  const double nSig0S = sig0_subEt ->Integral(1, nb0S);
  const double nSig2L = sig2D_lead2->Integral(1, nb2Lx, 1, nb2Ly);

  const double nSig2S = sig2D_sub2 ->Integral(1, nb2Sx, 1, nb2Sy);
  const double nSig2Both = sig2D_both2->Integral(1, nb2BX, 1, nb2BY);

  const double totalSig = nSig0L + nSig0S + nSig2L + nSig2S + nSig2Both;

  std::cout << "[MakeCombinedRateVsEff_AllFive] nSig0L,0S,2L,2S,both: "
            << nSig0L << " , " << nSig0S << " , "
            << nSig2L << " , " << nSig2S << " , "
            << nSig2Both << "\n";
  std::cout << "[MakeCombinedRateVsEff_AllFive] totalSig: " << totalSig << "\n";

  if (totalSig <= 0.0) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive: totalSig <= 0");
  }

  const double frac0L = nSig0L / totalSig;
  const double frac0S = nSig0S / totalSig;
  const double frac2L = nSig2L / totalSig;
  const double frac2S = nSig2S / totalSig;
  const double frac3D = nSig2Both / totalSig;

  // --- (1) Build per-category frontiers -----------------------------------
  auto front0L_raw = BuildFrontier1D(out0_leadEt, maxRateHz);
  auto front0S_raw = BuildFrontier1D(out0_subEt , maxRateHz);
  auto front2L_raw = BuildFrontier2D(out2D_lead2, maxRateHz);
  auto front2S_raw = BuildFrontier2D(out2D_sub2 , maxRateHz);
  auto front2D_raw = BuildFrontier2D(out2D_both2, maxRateHz);

  // Further restrict to per-category rate in [minRateHz, maxRateHz]
  auto front0L = FilterFrontierByRate(front0L_raw, minRateHz, maxRateHz);
  auto front0S = FilterFrontierByRate(front0S_raw, minRateHz, maxRateHz);
  auto front2L = FilterFrontierByRate(front2L_raw, minRateHz, maxRateHz);
  auto front2S = FilterFrontierByRate(front2S_raw, minRateHz, maxRateHz);
  auto front2D = FilterFrontierByRate(front2D_raw, minRateHz, maxRateHz);

  std::cout << "[MakeCombinedRateVsEff_AllFive] raw frontier sizes: "
            << "0L=" << front0L_raw.size()
            << ", 0S=" << front0S_raw.size()
            << ", 2L=" << front2L_raw.size()
            << ", 2S=" << front2S_raw.size()
            << ", 3D=" << front2D_raw.size() << "\n";

  std::cout << "[MakeCombinedRateVsEff_AllFive] filtered ("
            << minRateHz << "–" << maxRateHz << " Hz) frontier sizes: "
            << "0L=" << front0L.size()
            << ", 0S=" << front0S.size()
            << ", 2L=" << front2L.size()
            << ", 2S=" << front2S.size()
            << ", 3D=" << front2D.size() << "\n";

    // -------------------------------------------------------------------------
    // OPTION: completely disable the (1) 0/1-subjets subleading Et category
    // in the *numerator* (rate/eff contributions), but KEEP its signal events
    // in the denominator via totalSig (i.e. keep frac0S as computed above).
    // -------------------------------------------------------------------------
    const bool disableCat1_subEt = true;

    if (disableCat1_subEt) {
      std::cerr << "[MakeCombinedRateVsEff_AllFive] INFO: disabling category (1) out0_subEt "
                << "in combination (eff/rate set to 0), but keeping its signal fraction in denominator.\n";

      // Force the category-1 frontier to a single dummy point (no contribution).
      front0S.clear();
      front0S.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
    }

  // If a category has *no* usable points after filtering, treat it as "not used":
  // give it a single dummy point with eff=0, rate=0 so it contributes nothing.
  if (front0L.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 0L frontier empty, using dummy point (no contribution).\n";
    front0L.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
  }
  if (front0S.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 0S frontier empty, using dummy point (no contribution).\n";
    front0S.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
  }
  if (front2L.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 2L frontier empty, using dummy point (no contribution).\n";
    front2L.push_back(Cat2DPoint{0.0, 0.0, 0.0, 0.0, 0.0});
  }
  if (front2S.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 2S frontier empty, using dummy point (no contribution).\n";
    front2S.push_back(Cat2DPoint{0.0, 0.0, 0.0, 0.0, 0.0});
  }
  if (front2D.empty()) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive] WARNING: 3D frontier empty, using dummy point (no contribution).\n";
    front2D.push_back(Cat2DPoint{0.0, 0.0, 0.0, 0.0, 0.0});
  }

  // --- (2) Enumerate combinations over *filtered frontier* points only -----
  std::vector<GlobalPoint> globalFrontier;

  // We assume each frontier is sorted by rate ascending (as in BuildFrontier*).
  for (const auto& p0L : front0L) {
    const double eff0L_cat = p0L.eff_cat;
    const double rate0L    = p0L.rate_cat;
    const double err0L     = p0L.err_cat;
    const double etCut0L   = p0L.thrEt;

    for (const auto& p0S : front0S) {
      const double eff0S_cat = p0S.eff_cat;
      const double rate0S    = p0S.rate_cat;
      const double err0S     = p0S.err_cat;
      const double etCut0S   = p0S.thrEt;

      const double rate01 = rate0L + rate0S;
      if (rate01 > maxRateHz) break; // front0S sorted by rate

      for (const auto& p2L : front2L) {
        const double eff2L_cat = p2L.eff_cat;
        const double rate2L    = p2L.rate_cat;
        const double err2L     = p2L.err_cat;
        const double etCut2L   = p2L.thrEt;
        const double massCut2L = p2L.thrMass;

        const double rate012 = rate01 + rate2L;
        if (rate012 > maxRateHz) break; // front2L sorted by rate

        for (const auto& p2S : front2S) {
          const double eff2S_cat = p2S.eff_cat;
          const double rate2S    = p2S.rate_cat;
          const double err2S     = p2S.err_cat;
          const double etCut2S   = p2S.thrEt;
          const double massCut2S = p2S.thrMass;

          const double rate0123 = rate012 + rate2S;
          if (rate0123 > maxRateHz) break; // front2S sorted by rate

          for (const auto& p3D : front2D) {
            const double eff3D_cat = p3D.eff_cat;
            const double rate3D    = p3D.rate_cat;
            const double err3D     = p3D.err_cat;
            const double etCut3D_sub    = p3D.thrEt;
            const double massCut3D_lead = p3D.thrMass;

            const double rateTot = rate0123 + rate3D;
            if (rateTot > maxRateHz) break; // front3D sorted by rate

            // per-category global contributions
            const double eff0L_tot = eff0L_cat * frac0L;
            const double eff0S_tot = eff0S_cat * frac0S;
            const double eff2L_tot = eff2L_cat * frac2L;
            const double eff2S_tot = eff2S_cat * frac2S;
            const double eff3D_tot = eff3D_cat * frac3D;

            const double effTot =
                eff0L_tot + eff0S_tot + eff2L_tot + eff2S_tot + eff3D_tot;

            const double errTot =
                std::sqrt(err0L*err0L + err0S*err0S +
                          err2L*err2L + err2S*err2S + err3D*err3D);

            GlobalPoint gp;
            gp.eff = effTot;

            gp.eff_cat[0] = eff0L_cat;
            gp.eff_cat[1] = eff0S_cat;
            gp.eff_cat[2] = eff2L_cat;
            gp.eff_cat[3] = eff2S_cat;
            gp.eff_cat[4] = eff3D_cat;

            gp.eff_tot[0] = eff0L_tot;
            gp.eff_tot[1] = eff0S_tot;
            gp.eff_tot[2] = eff2L_tot;
            gp.eff_tot[3] = eff2S_tot;
            gp.eff_tot[4] = eff3D_tot;

            gp.rate        = rateTot;
            gp.rate_cat[0] = rate0L;
            gp.rate_cat[1] = rate0S;
            gp.rate_cat[2] = rate2L;
            gp.rate_cat[3] = rate2S;
            gp.rate_cat[4] = rate3D;

            gp.rateErr = errTot;

            gp.etCut0_lead      = etCut0L;
            gp.etCut0_sub       = etCut0S;
            gp.etCut_lead2      = etCut2L;
            gp.massCut_lead2    = massCut2L;
            gp.etCut_sub2       = etCut2S;
            gp.massCut_sub2     = massCut2S;
            gp.etCut_3D_sub     = etCut3D_sub;

            UpdateGlobalFrontier(globalFrontier, gp);
          }
        }
      }
    }
  }

  std::cout << "[MakeCombinedRateVsEff_AllFive] global frontier size: "
            << globalFrontier.size() << "\n";

  // --- (3) Build TGraphErrors from globalFrontier & pick best point -------
  auto* gCombined = new TGraphErrors(globalFrontier.size());
  gCombined->SetName("gRate_vsEff_all5_frontier");
  gCombined->SetTitle("Combined Trigger Rate vs Signal Efficiency;"
                      "Signal (hh#rightarrow4b) Efficiency (all 5 selections);"
                      "Total Estimated Background Rate [Hz]");

  GlobalRateEffOut5 out{};
  out.gRate_vsEff_combined = gCombined;

  // store signal fractions for legend
  out.fractionEventsPerCat[0] = frac0L;
  out.fractionEventsPerCat[1] = frac0S;
  out.fractionEventsPerCat[2] = frac2L;
  out.fractionEventsPerCat[3] = frac2S;
  out.fractionEventsPerCat[4] = frac3D;

  out.bestEff  = 0.0;  // default: no selection → eff=0
  out.bestRate = 0.0;  // default: rate=0

  for (size_t i = 0; i < globalFrontier.size(); ++i) {
    const auto& gp = globalFrontier[i];
    gCombined->SetPoint(i, gp.eff, gp.rate);
    gCombined->SetPointError(i, 0.0, gp.rateErr);

    if(gp.rate > maxRateHzPrint) continue; // ensure that the printed values are for a rate of the print value (10kHz)

    if (gp.eff > out.bestEff) {
      out.bestEff = gp.eff;
      out.bestRate = gp.rate;

      out.bestEtCut_0_lead    = gp.etCut0_lead;
      out.bestEtCut_0_sub     = gp.etCut0_sub;
      out.bestEtCut_lead2     = gp.etCut_lead2;
      out.bestMassCut_lead2   = gp.massCut_lead2;
      out.bestEtCut_sub2      = gp.etCut_sub2;
      out.bestMassCut_sub2    = gp.massCut_sub2;
      out.bestEtCut_3D_sub    = gp.etCut_3D_sub;
      out.bestMassCut_3D_lead = gp.massCut_3D_lead;
      out.bestMassCut_3D_sub  = gp.massCut_3D_sub;

      for (int c = 0; c < 5; ++c) {
        out.bestEff_cat[c]  = gp.eff_cat[c];
        out.bestEff_tot[c]  = gp.eff_tot[c];
        out.bestRate_cat[c] = gp.rate_cat[c];
      }
    }
  }

  return out;
}

struct GlobalRateEffOut5_EtOnly {
  TGraphErrors* gRate_vsEff_combined = nullptr;

  // signal fractions (from signal hist integrals)
  double fractionEventsPerCat[5] = {0,0,0,0,0};

  // best global point (max eff with total rate <= maxRateHz)
  double bestEff  = 0.0;
  double bestRate = 0.0;

  // best per-category Et thresholds at that point
  double bestEtCut[5] = {0,0,0,0,0};

  // per-category values at the best point
  double bestEff_cat[5]  = {0,0,0,0,0};   // efficiency within category
  double bestEff_tot[5]  = {0,0,0,0,0};   // contribution to total (eff_cat * frac)
  double bestRate_cat[5] = {0,0,0,0,0};   // background rate contribution [Hz]
};

// Pareto update: keep only points that are not dominated
// Dominated meaning: (eff <= other.eff AND rate >= other.rate) with at least one strict.
static inline bool Dominates(double effA, double rateA, double effB, double rateB) {
  const bool notWorse = (effA >= effB) && (rateA <= rateB);
  const bool strict   = (effA >  effB) || (rateA <  rateB);
  return notWorse && strict;
}

struct GlobalPoint1D {
  double eff = 0.0;
  double rate = 0.0;
  double rateErr = 0.0;

  double eff_cat[5]  = {0,0,0,0,0};
  double eff_tot[5]  = {0,0,0,0,0};
  double rate_cat[5] = {0,0,0,0,0};
  double etCut[5]    = {0,0,0,0,0};
};

static void UpdateGlobalFrontier1D(std::vector<GlobalPoint1D>& frontier,
                                   const GlobalPoint1D& cand)
{
  // If cand is dominated by any existing point, drop it.
  for (const auto& p : frontier) {
    if (Dominates(p.eff, p.rate, cand.eff, cand.rate)) return;
  }

  // Remove any points dominated by cand.
  frontier.erase(
    std::remove_if(frontier.begin(), frontier.end(),
                   [&](const GlobalPoint1D& p){
                     return Dominates(cand.eff, cand.rate, p.eff, p.rate);
                   }),
    frontier.end()
  );

  frontier.push_back(cand);

  // Optional: keep it tidy by sorting by rate ascending
  std::sort(frontier.begin(), frontier.end(),
            [](const GlobalPoint1D& a, const GlobalPoint1D& b){
              if (a.rate != b.rate) return a.rate < b.rate;
              return a.eff > b.eff;
            });
}

// ----------------------------------------------------------------------
// Et-only version: 5 categories, each category has ONLY an Et threshold.
// ----------------------------------------------------------------------
GlobalRateEffOut5_EtOnly MakeCombinedRateVsEff_AllFive_EtOnly(
    const RateEffOut& outCat0,
    const RateEffOut& outCat1,
    const RateEffOut& outCat2,
    const RateEffOut& outCat3,
    const RateEffOut& outCat4,
    TH1* sigCat0,
    TH1* sigCat1,
    TH1* sigCat2,
    TH1* sigCat3,
    TH1* sigCat4,
    double maxRateHzDraw,
    double maxRateHzPrint)
{
  const double minRateHz = 10.0;

  // Null checks (signal hists only used for fractions)
  if (!sigCat0 || !sigCat1 || !sigCat2 || !sigCat3 || !sigCat4) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive_EtOnly: null signal hist");
  }

  const RateEffOut* outs[5] = {&outCat0,&outCat1,&outCat2,&outCat3,&outCat4};
  for (int c = 0; c < 5; ++c) {
    if (!outs[c]->hEff_vsThr || !outs[c]->hRate_vsThr) {
      throw std::runtime_error("MakeCombinedRateVsEff_AllFive_EtOnly: null RateEffOut hists");
    }
  }

  // --- (0) Signal fractions per category -----------------------------------
  TH1* sigH[5] = {sigCat0,sigCat1,sigCat2,sigCat3,sigCat4};

  double nSig[5] = {0,0,0,0,0};
  double totalSig = 0.0;

  for (int c = 0; c < 5; ++c) {
    const int nb = sigH[c]->GetNbinsX();
    nSig[c] = sigH[c]->Integral(1, nb);
    totalSig += nSig[c];
  }

  std::cout << "[MakeCombinedRateVsEff_AllFive_EtOnly] nSig per cat: "
            << nSig[0] << ", " << nSig[1] << ", " << nSig[2] << ", "
            << nSig[3] << ", " << nSig[4] << "\n";
  std::cout << "[MakeCombinedRateVsEff_AllFive_EtOnly] totalSig: " << totalSig << "\n";

  if (totalSig <= 0.0) {
    throw std::runtime_error("MakeCombinedRateVsEff_AllFive_EtOnly: totalSig <= 0");
  }

  double frac[5] = {0,0,0,0,0};
  for (int c = 0; c < 5; ++c) frac[c] = nSig[c] / totalSig;

  // --- (1) Build per-category 1D frontiers ---------------------------------
  auto front_raw0 = BuildFrontier1D(outCat0, maxRateHzDraw);
  auto front_raw1 = BuildFrontier1D(outCat1, maxRateHzDraw);
  auto front_raw2 = BuildFrontier1D(outCat2, maxRateHzDraw);
  auto front_raw3 = BuildFrontier1D(outCat3, maxRateHzDraw);
  auto front_raw4 = BuildFrontier1D(outCat4, maxRateHzDraw);

  auto front0 = FilterFrontierByRate(front_raw0, minRateHz, maxRateHzDraw);
  auto front1 = FilterFrontierByRate(front_raw1, minRateHz, maxRateHzDraw);
  auto front2 = FilterFrontierByRate(front_raw2, minRateHz, maxRateHzDraw);
  auto front3 = FilterFrontierByRate(front_raw3, minRateHz, maxRateHzDraw);
  auto front4 = FilterFrontierByRate(front_raw4, minRateHz, maxRateHzDraw);

  std::cout << "[MakeCombinedRateVsEff_AllFive_EtOnly] raw frontier sizes: "
            << "c0=" << front_raw0.size()
            << ", c1=" << front_raw1.size()
            << ", c2=" << front_raw2.size()
            << ", c3=" << front_raw3.size()
            << ", c4=" << front_raw4.size() << "\n";

  std::cout << "[MakeCombinedRateVsEff_AllFive_EtOnly] filtered ("
            << minRateHz << "–" << maxRateHzDraw << " Hz) frontier sizes: "
            << "c0=" << front0.size()
            << ", c1=" << front1.size()
            << ", c2=" << front2.size()
            << ", c3=" << front3.size()
            << ", c4=" << front4.size() << "\n";
  
  // -------------------------------------------------------------------------
  // OPTION: completely disable the (1) category (outCat1) in the *numerator*
  // (rate/eff contributions), but KEEP its signal events in the denominator
  // via totalSig (i.e. keep frac[1] as computed above).
  // -------------------------------------------------------------------------
  const bool disableCat1 = true;

  if (disableCat1) {
    std::cerr << "[MakeCombinedRateVsEff_AllFive_EtOnly] INFO: disabling category 1 contribution "
              << "(eff/rate set to 0), but keeping its signal fraction in denominator.\n";

    // Force category-1 frontier to a single dummy point (no contribution).
    front1.clear();
    front1.push_back(Cat1DPoint{0.0, 0.0, 0.0, 0.0});
  }

  // If any category has no usable points, give it a dummy point (no contribution).
  auto ensure_nonempty = [&](std::vector<Cat1DPoint>& f, int c){
    if (f.empty()) {
      std::cerr << "[MakeCombinedRateVsEff_AllFive_EtOnly] WARNING: cat " << c
                << " frontier empty, using dummy point.\n";
      f.push_back(Cat1DPoint{0.0,0.0,0.0,0.0});
    }
  };
  ensure_nonempty(front0,0);
  ensure_nonempty(front1,1);
  ensure_nonempty(front2,2);
  ensure_nonempty(front3,3);
  ensure_nonempty(front4,4);

  // --- (2) Enumerate combinations over filtered frontier points -------------
  std::vector<GlobalPoint1D> globalFrontier;

  for (const auto& p0 : front0) {
    const double rate0 = p0.rate_cat;
    const double eff0  = p0.eff_cat;
    const double err0  = p0.err_cat;
    const double cut0  = p0.thrEt;

    for (const auto& p1 : front1) {
      const double rate01 = rate0 + p1.rate_cat;
      if (rate01 > maxRateHzDraw) break; // assumes front1 sorted by rate asc
      const double err01 = std::sqrt(err0*err0 + p1.err_cat*p1.err_cat);

      for (const auto& p2 : front2) {
        const double rate012 = rate01 + p2.rate_cat;
        if (rate012 > maxRateHzDraw) break;
        const double err012 = std::sqrt(err01*err01 + p2.err_cat*p2.err_cat);

        for (const auto& p3 : front3) {
          const double rate0123 = rate012 + p3.rate_cat;
          if (rate0123 > maxRateHzDraw) break;
          const double err0123 = std::sqrt(err012*err012 + p3.err_cat*p3.err_cat);

          for (const auto& p4 : front4) {
            const double rateTot = rate0123 + p4.rate_cat;
            if (rateTot > maxRateHzDraw) break;
            const double errTot  = std::sqrt(err0123*err0123 + p4.err_cat*p4.err_cat);

            const double eff_cat[5]  = {eff0, p1.eff_cat,  p2.eff_cat,  p3.eff_cat,  p4.eff_cat};
            const double rate_cat[5] = {rate0,p1.rate_cat, p2.rate_cat, p3.rate_cat, p4.rate_cat};
            const double cut[5]      = {cut0,p1.thrEt,     p2.thrEt,     p3.thrEt,     p4.thrEt};

            double eff_tot[5] = {0,0,0,0,0};
            double effTot = 0.0;
            for (int c = 0; c < 5; ++c) {
              eff_tot[c] = eff_cat[c] * frac[c];
              effTot += eff_tot[c];
            }

            GlobalPoint1D gp;
            gp.eff = effTot;
            gp.rate = rateTot;
            gp.rateErr = errTot;
            for (int c = 0; c < 5; ++c) {
              gp.eff_cat[c]  = eff_cat[c];
              gp.eff_tot[c]  = eff_tot[c];
              gp.rate_cat[c] = rate_cat[c];
              gp.etCut[c]    = cut[c];
            }

            UpdateGlobalFrontier1D(globalFrontier, gp);
          }
        }
      }
    }
  }

  std::cout << "[MakeCombinedRateVsEff_AllFive_EtOnly] global frontier size: "
            << globalFrontier.size() << "\n";

  // --- (3) Make graph + pick best point ------------------------------------
  auto* gCombined = new TGraphErrors(globalFrontier.size());
  gCombined->SetName("gRate_vsEff_all5_EtOnly_frontier");
  gCombined->SetTitle("Combined Trigger Rate vs Signal Efficiency (E_{T}-only);"
                      "Signal Efficiency ;"
                      "Total Estimated Background Rate [Hz]");

  GlobalRateEffOut5_EtOnly out;
  out.gRate_vsEff_combined = gCombined;

  for (int c = 0; c < 5; ++c) out.fractionEventsPerCat[c] = frac[c];

  out.bestEff  = 0.0;
  out.bestRate = 0.0;

  for (size_t i = 0; i < globalFrontier.size(); ++i) {
    const auto& gp = globalFrontier[i];
    gCombined->SetPoint(i, gp.eff, gp.rate);
    gCombined->SetPointError(i, 0.0, gp.rateErr);

    if(gp.rate > maxRateHzPrint) continue; // ensure that the printed values are for a rate of the print value (10kHz)

    if (gp.eff > out.bestEff) {
      out.bestEff  = gp.eff;
      out.bestRate = gp.rate;
      
      for (int c = 0; c < 5; ++c) {
        out.bestEtCut[c]    = gp.etCut[c];
        out.bestEff_cat[c]  = gp.eff_cat[c];
        out.bestEff_tot[c]  = gp.eff_tot[c];
        out.bestRate_cat[c] = gp.rate_cat[c];
      }
    }
  }

  return out;
}


// ----------------------------------------------------------------------
// Et-only version: 8 categories, each category has ONLY an Et threshold.
// - Disables category 0 in the numerator (eff/rate contributions set to 0)
//   BUT keeps its signal events in the efficiency denominator via frac[0].
// ----------------------------------------------------------------------

struct GlobalPoint1D_8 {
  double eff=0.0;
  double rate=0.0;
  double rateErr=0.0;
  double eff_cat[8]={0,0,0,0,0,0,0,0};
  double eff_tot[8]={0,0,0,0,0,0,0,0};
  double rate_cat[8]={0,0,0,0,0,0,0,0};
  double etCut[8]={0,0,0,0,0,0,0,0};
};

struct GlobalRateEffOut8_EtOnly {
  TGraphErrors* gRate_vsEff_combined=nullptr;

  double fractionEventsPerCat[8]={0,0,0,0,0,0,0,0};

  double bestEff=0.0;
  double bestRate=0.0;

  double bestEtCut[8]={0,0,0,0,0,0,0,0};

  double bestEff_cat[8]={0,0,0,0,0,0,0,0};
  double bestEff_tot[8]={0,0,0,0,0,0,0,0};
  double bestRate_cat[8]={0,0,0,0,0,0,0,0};
};

// Pareto update: keep only non-dominated points (higher eff, lower rate).
static inline void UpdateGlobalFrontier1D_8(std::vector<GlobalPoint1D_8>& frontier,const GlobalPoint1D_8& cand){
  // Reject if dominated by an existing point
  for(const auto& p:frontier){
    if(p.eff>=cand.eff && p.rate<=cand.rate) return;
  }
  // Remove points dominated by candidate
  frontier.erase(std::remove_if(frontier.begin(),frontier.end(),
    [&](const GlobalPoint1D_8& p){return (cand.eff>=p.eff && cand.rate<=p.rate);}),
    frontier.end());
  frontier.push_back(cand);
}

GlobalRateEffOut8_EtOnly MakeCombinedRateVsEff_AllEight_EtOnly(
  const RateEffOut& outCat0,
  const RateEffOut& outCat1,
  const RateEffOut& outCat2,
  const RateEffOut& outCat3,
  const RateEffOut& outCat4,
  const RateEffOut& outCat5,
  const RateEffOut& outCat6,
  const RateEffOut& outCat7,
  TH1* sigCat0,
  TH1* sigCat1,
  TH1* sigCat2,
  TH1* sigCat3,
  TH1* sigCat4,
  TH1* sigCat5,
  TH1* sigCat6,
  TH1* sigCat7,
  double maxRateHzDraw,
  double maxRateHzPrint)
{
  const double minRateHz=10.0;

  // ---- null checks ----
  TH1* sigH[8]={sigCat0,sigCat1,sigCat2,sigCat3,sigCat4,sigCat5,sigCat6,sigCat7};
  for(int c=0;c<8;++c){
    if(!sigH[c]) throw std::runtime_error("MakeCombinedRateVsEff_AllEight_EtOnly: null signal hist");
  }

  const RateEffOut* outs[8]={&outCat0,&outCat1,&outCat2,&outCat3,&outCat4,&outCat5,&outCat6,&outCat7};
  for(int c=0;c<8;++c){
    if(!outs[c]->hEff_vsThr||!outs[c]->hRate_vsThr){
      throw std::runtime_error("MakeCombinedRateVsEff_AllEight_EtOnly: null RateEffOut hists");
    }
  }

  // ---- (0) signal fractions per category ----
  double nSig[8]={0,0,0,0,0,0,0,0};
  double totalSig=0.0;
  for(int c=0;c<8;++c){
    const int nb=sigH[c]->GetNbinsX();
    nSig[c]=sigH[c]->Integral(1,nb);
    totalSig+=nSig[c];
  }

  std::cout<<"[MakeCombinedRateVsEff_AllEight_EtOnly] nSig per cat: "
           <<nSig[0]<<", "<<nSig[1]<<", "<<nSig[2]<<", "<<nSig[3]<<", "
           <<nSig[4]<<", "<<nSig[5]<<", "<<nSig[6]<<", "<<nSig[7]<<"\n";
  std::cout<<"[MakeCombinedRateVsEff_AllEight_EtOnly] totalSig: "<<totalSig<<"\n";

  if(totalSig<=0.0) throw std::runtime_error("MakeCombinedRateVsEff_AllEight_EtOnly: totalSig <= 0");

  double frac[8]={0,0,0,0,0,0,0,0};
  for(int c=0;c<8;++c) frac[c]=nSig[c]/totalSig;

  // ---- (1) build per-category 1D frontiers ----
  auto raw0=BuildFrontier1D(outCat0,maxRateHzDraw);
  auto raw1=BuildFrontier1D(outCat1,maxRateHzDraw);
  auto raw2=BuildFrontier1D(outCat2,maxRateHzDraw);
  auto raw3=BuildFrontier1D(outCat3,maxRateHzDraw);
  auto raw4=BuildFrontier1D(outCat4,maxRateHzDraw);
  auto raw5=BuildFrontier1D(outCat5,maxRateHzDraw);
  auto raw6=BuildFrontier1D(outCat6,maxRateHzDraw);
  auto raw7=BuildFrontier1D(outCat7,maxRateHzDraw);

  auto f0=FilterFrontierByRate(raw0,10,11);
  auto f1=FilterFrontierByRate(raw1,10,7500);
  auto f2=FilterFrontierByRate(raw2,10,7500);
  auto f3=FilterFrontierByRate(raw3,10,7500);
  auto f4=FilterFrontierByRate(raw4,10,7500);
  auto f5=FilterFrontierByRate(raw5,10,500);
  auto f6=FilterFrontierByRate(raw6,10,500);
  auto f7=FilterFrontierByRate(raw7,0,500);

  auto sort_by_rate=[](std::vector<Cat1DPoint>& f){
    std::sort(f.begin(),f.end(),[](const Cat1DPoint& a,const Cat1DPoint& b){return a.rate_cat<b.rate_cat;});
  };
  sort_by_rate(f0);sort_by_rate(f1);sort_by_rate(f2);sort_by_rate(f3);
  sort_by_rate(f4);sort_by_rate(f5);sort_by_rate(f6);sort_by_rate(f7);

  std::cout<<"[MakeCombinedRateVsEff_AllEight_EtOnly] raw frontier sizes: "
           <<"c0="<<raw0.size()<<", c1="<<raw1.size()<<", c2="<<raw2.size()<<", c3="<<raw3.size()
           <<", c4="<<raw4.size()<<", c5="<<raw5.size()<<", c6="<<raw6.size()<<", c7="<<raw7.size()<<"\n";
  std::cout<<"[MakeCombinedRateVsEff_AllEight_EtOnly] filtered ("<<minRateHz<<"–"<<maxRateHzDraw<<" Hz) frontier sizes: "
           <<"c0="<<f0.size()<<", c1="<<f1.size()<<", c2="<<f2.size()<<", c3="<<f3.size()
           <<", c4="<<f4.size()<<", c5="<<f5.size()<<", c6="<<f6.size()<<", c7="<<f7.size()<<"\n";

  // -------------------------------------------------------------------------
  // Disable category 0 in the *numerator* (rate/eff contributions), but keep
  // its signal fraction in denominator via frac[0].
  // -------------------------------------------------------------------------
  const bool disableCat0=true;
  if(disableCat0){
    std::cerr<<"[MakeCombinedRateVsEff_AllEight_EtOnly] INFO: disabling category 0 contribution "
             <<"(eff/rate set to 0), but keeping its signal fraction in denominator.\n";
    f0.clear();
    f0.push_back(Cat1DPoint{0.0,0.0,0.0,0.0});
  }

  auto ensure_nonempty=[&](std::vector<Cat1DPoint>& f,int c){
    if(f.empty()){
      std::cerr<<"[MakeCombinedRateVsEff_AllEight_EtOnly] WARNING: category "<<c<<" frontier empty, using dummy point.\n";
      f.push_back(Cat1DPoint{0.0,0.0,0.0,0.0});
    }
  };
  ensure_nonempty(f0,0);ensure_nonempty(f1,1);ensure_nonempty(f2,2);ensure_nonempty(f3,3);
  ensure_nonempty(f4,4);ensure_nonempty(f5,5);ensure_nonempty(f6,6);ensure_nonempty(f7,7);

  // ---- (2) enumerate combinations (recursive, prunes by rate) ----
  std::vector<GlobalPoint1D_8> globalFrontier;

  std::vector<Cat1DPoint> F[8]={f0,f1,f2,f3,f4,f5,f6,f7};

  double eff_cat[8]={0,0,0,0,0,0,0,0};
  double eff_tot[8]={0,0,0,0,0,0,0,0};
  double rate_cat[8]={0,0,0,0,0,0,0,0};
  double cut[8]={0,0,0,0,0,0,0,0};

  std::function<void(int,double,double,double)> dfs;
  dfs=[&](int c,double rate,double err2,double effSum){
    if(rate>maxRateHzDraw) return;
    if(c==8){
      GlobalPoint1D_8 gp;
      gp.eff=effSum;
      gp.rate=rate;
      gp.rateErr=std::sqrt(err2);
      for(int i=0;i<8;++i){
        gp.eff_cat[i]=eff_cat[i];
        gp.eff_tot[i]=eff_tot[i];
        gp.rate_cat[i]=rate_cat[i];
        gp.etCut[i]=cut[i];
      }
      UpdateGlobalFrontier1D_8(globalFrontier,gp);
      return;
    }

    for(const auto& p:F[c]){
      const double newRate=rate+p.rate_cat;
      if(newRate>maxRateHzDraw) break; // relies on sorted-by-rate
      const double newErr2=err2+p.err_cat*p.err_cat;

      eff_cat[c]=p.eff_cat;
      rate_cat[c]=p.rate_cat;
      cut[c]=p.thrEt;

      eff_tot[c]=eff_cat[c]*frac[c];
      dfs(c+1,newRate,newErr2,effSum+eff_tot[c]);
    }
  };

  dfs(0,0.0,0.0,0.0);

  std::cout<<"[MakeCombinedRateVsEff_AllEight_EtOnly] global frontier size: "<<globalFrontier.size()<<"\n";

  // ---- (3) make graph + pick best point (max eff with rate <= maxRateHzPrint) ----
  auto* gCombined=new TGraphErrors(globalFrontier.size());
  gCombined->SetName("gRate_vsEff_all8_EtOnly_frontier");
  gCombined->SetTitle("Combined Trigger Rate vs Signal Efficiency (E_{T}-only);Signal Efficiency;Total Estimated Background Rate [Hz]");

  GlobalRateEffOut8_EtOnly out;
  out.gRate_vsEff_combined=gCombined;

  for(int c=0;c<8;++c) out.fractionEventsPerCat[c]=frac[c];

  out.bestEff=0.0;
  out.bestRate=0.0;

  for(size_t i=0;i<globalFrontier.size();++i){
    const auto& gp=globalFrontier[i];
    gCombined->SetPoint(i,gp.eff,gp.rate);
    gCombined->SetPointError(i,0.0,gp.rateErr);

    if(gp.rate>maxRateHzPrint) continue;
    if(gp.eff>out.bestEff){
      out.bestEff=gp.eff;
      out.bestRate=gp.rate;
      for(int c=0;c<8;++c){
        out.bestEtCut[c]=gp.etCut[c];
        out.bestEff_cat[c]=gp.eff_cat[c];
        out.bestEff_tot[c]=gp.eff_tot[c];
        out.bestRate_cat[c]=gp.rate_cat[c];
      }
    }
  }

  return out;
}



constexpr unsigned int nJZSlices_ = 10;


// Constants for Event Display Generation
constexpr unsigned int evDisplayMinSig_ = 15;
constexpr unsigned int evDisplay0Sig_ = evDisplayMinSig_ + 1;
constexpr unsigned int evDisplay1Sig_ = evDisplay0Sig_ + 1;
constexpr unsigned int evDisplay2Sig_ = evDisplay1Sig_ + 1;
constexpr unsigned int evDisplay3Sig_ = evDisplay2Sig_ + 1;
constexpr unsigned int evDisplay4Sig_ = evDisplay3Sig_ + 1;
constexpr unsigned int evDisplayMaxSig_ = evDisplay4Sig_;

constexpr unsigned int evDisplayMinBack_ = 10;
constexpr unsigned int evDisplay0Back_ = evDisplayMinBack_ + 1;
constexpr unsigned int evDisplay1Back_ = evDisplay0Back_ + 1;
constexpr unsigned int evDisplay2Back_ = evDisplay1Back_ + 1;
constexpr unsigned int evDisplay3Back_ = evDisplay2Back_ + 1;
constexpr unsigned int evDisplay4Back_ = evDisplay3Back_ + 1;
constexpr unsigned int evDisplayMaxBack_ = evDisplay4Back_;

bool displayEv0Sig_ = false;
bool displayEv1Sig_ = false;
bool displayEv2Sig_ = false;
bool displayEv3Sig_ = false;
bool displayEv4Sig_ = false;

bool displayEv0Back_ = false;
bool displayEv1Back_ = false;
bool displayEv2Back_ = false;
bool displayEv3Back_ = false;
bool displayEv4Back_ = false;

const double mH_ = 125.0;

constexpr double et_granularity_ = 0.125;
constexpr unsigned int et_bit_length_ = 13;
constexpr unsigned int eta_bit_length_ = 8;
constexpr unsigned int phi_bit_length_ = 6;
constexpr unsigned int psi_R_bit_length_ = 10; 
constexpr unsigned int padded_zeroes_length_ = 64 - eta_bit_length_ - et_bit_length_ - phi_bit_length_ - psi_R_bit_length_;
constexpr double phi_min_ = -3.2;
constexpr double phi_max_ = 3.2;
constexpr double eta_min_ = -5.0;
constexpr double eta_max_ = 5.0;
constexpr double eta_granularity_ = 0.0390625;
constexpr double phi_granularity_ = 0.1;
constexpr unsigned int et_min_ = 0;
constexpr unsigned int et_max_ = 1024;
const bool useMax_ = false;
const unsigned nSeeds_ = 2;

inline double undigitize_phi(const std::bitset<phi_bit_length_>& phi_bits) {
    return phi_min_ + phi_bits.to_ulong() * (6.4 / 256.0);
}

inline double undigitize_eta(const std::bitset<eta_bit_length_>& eta_bits) {
    return eta_min_ + eta_bits.to_ulong() * (10.0 / 2048.0);
}

inline double undigitize_et(const std::bitset<et_bit_length_>& et_bits) {
    return et_bits.to_ulong() * et_granularity_;
}

inline double undigitize_nmio(const std::bitset<psi_R_bit_length_>& nmio_bits) {
    return nmio_bits.to_ulong() * 0.0048828125; // 5 / 2^psi_R_bit_length_
}

// Function to scale and digitize a value, returning the result as a binary string
template <int bit_length>
inline std::bitset<bit_length > digitize(double value, double min_val, double max_val) {
    // Ensure the value is within range
    if (value < min_val || value > max_val) {
        std::cerr << "Value is out of range" << "\n";
    }
    //std::cout << "value : " << value << "\n";

    // Calculate scale factor
    double scale = (std::pow(2, bit_length)) / (max_val - min_val);

    //std::cout << "scale: " << scale << "\n";
    
    // Calculate the digitized value and round it
    int digitized_value = static_cast<int>(std::round((value - min_val) * scale));

    //std::cout << "digitized value: " << digitized_value << "\n";
    
    // Return the digitized value as a binary string (with leading zeros to fit the bit length)
    std::bitset<bit_length > bin(digitized_value);
    //std::pair<std::bitset<bit_length >, unsigned int> string_dig_pair(bin, digitized_value);
    return bin; // Extract only the relevant bits
}

double calcDeltaR2(double eta1, double phi1, double eta2, double phi2) {
    //std::cout << "eta1: " << eta1 << " eta2: " << eta2 << " phi1: " << phi1 << " phi2: " << phi2 << "\n";
    double dEta = eta1 - eta2;
    double dPhi = std::abs(phi1 - phi2);
    
    // Ensure Δφ is within [-π, π] range
    if (dPhi > M_PI) {
        dPhi = 2 * M_PI - dPhi;
    }
    //std::cout << "dr2: " << dEta * dEta + dPhi * dPhi << "\n";
    return dEta * dEta + dPhi * dPhi;
}

bool energyMax(double et, double et_cutoff){
    return et <= et_cutoff;
}

bool energyMin(double et, double et_cutoff){
    return et >= et_cutoff;
}

inline void redirect_output_to_file() {
    // Open the file in write mode
    freopen("output.txt", "w", stdout);  // This will redirect stdout to output.txt
}

#include <TLatex.h>
#include <TH2.h>

void DrawBinValues(TH2* h, bool sciNot, double textSize = 0.03) {
    TLatex lat;
    lat.SetTextAlign(22);   // center horizontally & vertically
    lat.SetTextSize(textSize);
    lat.SetTextFont(42);
    lat.SetTextColor(kBlack);

    for (int ix = 1; ix <= h->GetNbinsX(); ++ix) {
        for (int iy = 1; iy <= h->GetNbinsY(); ++iy) {

            double val = h->GetBinContent(ix, iy);
            if (val == 0) continue;  // skip empty bins (optional)

            double x = h->GetXaxis()->GetBinCenter(ix);
            double y = h->GetYaxis()->GetBinCenter(iy);
            if(sciNot) lat.DrawLatex(x, y, Form("%.1e", val));  // scientific notation
            else lat.DrawLatex(x, y, Form("%.3f", val));  // scientific notation
        
        }
    }
}


void SetPlotStyle();
void mySmallText(Double_t x, Double_t y, Color_t color, char* text);

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
    gStyle->SetPadRightMargin(0.15);
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

//#include "../algorithm/constants.h"
