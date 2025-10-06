import ROOT
ROOT.xAOD.Init().isSuccess()
import os

fileNames = [
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ0/DAOD_JETM42.BACKGROUND_JZ0_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ1/DAOD_JETM42.BACKGROUND_JZ1_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ2/DAOD_JETM42.BACKGROUND_JZ2_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ3/DAOD_JETM42.BACKGROUND_JZ3_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ4/DAOD_JETM42.BACKGROUND_JZ4_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ5/DAOD_JETM42.BACKGROUND_JZ5_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ6/DAOD_JETM42.BACKGROUND_JZ6_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ7/DAOD_JETM42.BACKGROUND_JZ7_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ8/DAOD_JETM42.BACKGROUND_JZ8_DAOD_JETM42.root",
    "/data/larsonma/LargeRadiusJets/datasets/DAOD_TrigGepPerf/Background_jj_JZ9/DAOD_JETM42.BACKGROUND_JZ9_DAOD_JETM42.root"
]

# open output file for writing
with open("sumOfWeights.txt", "w") as out:
    for file in fileNames:
        f = ROOT.TFile.Open(file)
        tree = ROOT.xAOD.MakeTransientMetaTree(f, "MetaData")
        tree.GetEntry(0)
        cbks = tree.CutBookkeepers

        maxCycle = -1
        theCbk = None
        for cbk in cbks:
            if cbk.name() != "AllExecutedEvents":
                continue
            if cbk.inputStream() != "StreamAOD":
                continue
            if cbk.cycle() > maxCycle:
                maxCycle = cbk.cycle()
                theCbk = cbk

        if theCbk:
            this_sum = theCbk.sumOfEventWeights()
            out.write(f"{os.path.basename(file)} {this_sum}\n")
            print(f"{file}: Sum of Weights = {this_sum}")
        else:
            out.write(f"{os.path.basename(file)} 0.0\n")
            print(f"{file}: No matching CutBookkeeper found!")

        f.Close()

ROOT.xAOD.ClearTransientTrees()
