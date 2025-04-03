#!/usr/bin/env python

# Set up ROOT and RootCore
import ROOT
import os
import numpy as np
import matplotlib.pyplot as plt

signalBool = False

higgsPtCut = True

def reset_event_counter(eventCounter): # reset event counter to keep track of kept event number after higgs cuts
    if eventCounter == 0:
        eventCounter = 0
    else: 
        eventCounter -= 1

# Plotting the heatmaps
def plot_heatmap(data, eta_bins, phi_bins, title, filename, log = False, Higgs1Pt = 0, Higgs2Pt = 0, Higgs1Eta = 0, Higgs2Eta = 0, Higgs1Phi = 0, Higgs2Phi = 0, signalBool = False):
    plt.figure(figsize=(8, 6))
    plt.imshow(data.T, origin='lower', aspect='auto',
               extent=[eta_bins[0], eta_bins[-1], phi_bins[0], phi_bins[-1]],
               cmap='viridis')
    
    if log: 
        plt.colorbar(label='Log_10(Sum of $E_T$)')
    else: 
        plt.colorbar(label='Sum of $E_T$ [GeV]')
    plt.xlabel('Eta')
    plt.ylabel('Phi')
    plt.title(title)
    if signalBool:
        # Create a legend box outside the plot
        legend_text = (f"Higgs 1: pT={Higgs1Pt:.2f} GeV, Eta={Higgs1Eta:.2f}, Phi={Higgs1Phi:.2f}\n"
                    f"Higgs 2: pT={Higgs2Pt:.2f} GeV, Eta={Higgs2Eta:.2f}, Phi={Higgs2Phi:.2f}")
        
        plt.figtext(1.02, 0.5, legend_text, ha="left", va="center", fontsize=10, bbox=dict(facecolor='white', edgecolor='black'))
    plt.savefig(filename)
    plt.close()

# Initialize the xAOD infrastructure
#ROOT.gSystem.Load("libxAODRootAccess")
#ROOT.xAOD.Init()

# Set up the input file directory
if signalBool:
    fileDir = "/home/larsonma/LargeRadiusJets/data/datasets/Signal_HHbbb/mc21_14TeV.537540.MGPy8EG_hh_bbbb_vbf_novhh_5fs_l1cvv1cv1.recon.AOD.e8557_s4422_r16130/"
else: 
    fileDir = "/home/larsonma/LargeRadiusJets/data/datasets/Background_jj_JZ3/mc21_14TeV.801168.Py8EG_A14NNPDF23LO_jj_JZ3.recon.AOD.e8557_s4422_r16130"


# Get a list of all ROOT files in the directory
fileNames = [os.path.join(fileDir, f) for f in os.listdir(fileDir) if f.endswith(".pool.root.1")]

if not fileNames:
    print("No ROOT files found in the specified directory.")
    exit(1)

# Initialize arrays to store phi, eta, and Et values
topo_phi_values = []
topo_eta_values = []
topo_et_values = []

topo_422_phi_values = []
topo_422_eta_values = []
topo_422_et_values = []

gfex_smallr_phi_values = []
gfex_smallr_eta_values = []
gfex_smallr_et_values = []

jfex_smallr_phi_values = []
jfex_smallr_eta_values = []
jfex_smallr_et_values = []

higgs_pt_values = []  # List to store pt values
jet_pt_values = []
higgs_pt_values_after_cut = []

# Heatmap bin definitions
phi_bins = np.linspace(-3.2, 3.2, 65)  # 64 bins
eta_bins = np.linspace(-5.0, 5.0, 101)  # 100 bins
if signalBool:
    output_file_topo422 = "/home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_hh_bbbb_vbf_novhh_topo422.dat"
    output_file_gfex = "/home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_hh_bbbb_vbf_novhh_gfex_smallrj.dat"
    output_file_jfex = "/home/larsonma/LargeRadiusJets/data/MemPrints/jFex/mc21_14TeV_hh_bbbb_vbf_novhh_jfex_smallrj.dat"
else:
    output_file_topo422 = "/home/larsonma/LargeRadiusJets/data/MemPrints/CaloTopo_422/mc21_14TeV_jj_JZ3_topo422.dat"
    output_file_gfex = "/home/larsonma/LargeRadiusJets/data/MemPrints/gFex/mc21_14TeV_jj_JZ3_gfex_smallrj.dat"
    output_file_jfex = "/home/larsonma/LargeRadiusJets/data/MemPrints/jFex/mc21_14TeV_jj_JZ3_jfex_smallrj.dat"

# Function to scale and digitize a value
def digitize(value, bit_length, min_val, max_val):
    # Ensure the value is within range
    if value < min_val or value > max_val:
        raise ValueError(f"Value {value} is out of range ({min_val}, {max_val})")
    scale = (2**bit_length - 1) / (max_val - min_val)
    return int(round((value - min_val) * scale))

# Parameters
phi_bit_length = 8
phi_min = -3.2
phi_max = 3.2

eta_bit_length = 11
eta_min = -5.0
eta_max = 5.0

et_bit_length = 13
et_min = 0
et_max = 2048
et_granularity = 0.25  # GeV per bit

events_per_file = 100

higgs_min_pt = 100
higgs_max_pt = 200

fileIt = 0
eventCounter = -1

# Process each file
with open(output_file_topo422, "w") as f_topo:
    with open(output_file_gfex, "w") as f_gfex:
        with open(output_file_jfex, "w") as f_jfex:
            
            for fileName in fileNames:
                #if fileIt > 0:
                #    break
                print(f"Processing file: {fileName}")

                # Open the file and make the "transient tree"
                f = ROOT.TFile.Open(fileName)
                t = ROOT.xAOD.MakeTransientTree(f, 'CollectionTree')

                # Print the number of events in the file
                print('  Number of input events: %s' % t.GetEntries())
                # Loop through all events in the file
                for entry in range(t.GetEntries()):
                    higgs_1_pt = 0
                    higgs_2_pt = 0
                    higgs_1_eta = 0
                    higgs_2_eta = 0
                    higgs_1_phi = 0
                    higgs_2_phi = 0
                    higgs_counter = 0
                    higgs_average_pt = 0
                    eventCounter += 1
                    higgs_pt_values_by_event = []  # List to store pt values
                    higgs_passes_cut = False
                    t.GetEntry(entry)
                    iEvt = eventCounter # 100 events per file
                    print("entry, fileIt, iEvt:", eventCounter, fileIt, iEvt)

                    topo_phi_values_by_event = []
                    topo_eta_values_by_event = []
                    topo_et_values_by_event = []

                    topo_422_phi_values_by_event = []
                    topo_422_eta_values_by_event = []
                    topo_422_et_values_by_event = []

                    gfex_smallr_phi_values_by_event = []
                    gfex_smallr_eta_values_by_event = []
                    gfex_smallr_et_values_by_event = []

                    jfex_smallr_phi_values_by_event = []
                    jfex_smallr_eta_values_by_event = []
                    jfex_smallr_et_values_by_event = []

                    # Heatmap storage
                    topo_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    topo_422_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    gfex_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    jfex_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))

                    topo_log_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    topo_422_log_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    gfex_log_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    jfex_log_heatmap = np.zeros((len(eta_bins) - 1, len(phi_bins) - 1))
                    
                    if iEvt == 0: 
                        # Loop over all collections in the transient tree (only for the first event of the first file)
                        for key in t.GetListOfBranches():
                            collection_name = key.GetName()
                            collection = getattr(t, collection_name, None)

                            if collection:
                                print(f'  Collection: {collection_name}')

                    sum_et = 0.0

                    topo_it = 0

                    #if iEvt <= 9:
                    truthIt = 0
                    for el in t.InTimeAntiKt4TruthJets:
                        print("truth jet pt:", el.pt() / 1000)
                        jet_pt_values.append(el.pt() / 1000)
                    for el in t.TruthParticles:
                        if el.pdgId() == 25 and el.status() == 22:
                            higgs_counter += 1
                            if higgs_counter == 1:
                                higgs_1_pt = el.pt()/1000
                                higgs_1_eta = el.eta()
                                higgs_1_phi = el.phi()
                            if higgs_counter == 2:
                                higgs_2_pt = el.pt()/1000
                                higgs_2_eta = el.eta()
                                higgs_2_phi = el.phi()
                            higgs_average_pt = (higgs_1_pt + higgs_2_pt) / 2.0
                            higgs_pt_values.append(el.pt()/1000)
                            higgs_pt_values_by_event.append(el.pt()/1000)
                        #print (" truthIt: ", truthIt)
                        #if el.e() >= 10000.0:
                            #print('  PDG ID = %d, Eta = %g, Phi = %g, energy [GeV] = %g, Pt [GeV] = %g, iEvt = %g, status = %g' %  
                            #(el.pdgId(), el.eta(), el.phi(), el.e()/1000.0, el.pt()/1000, iEvt, el.status()))
                        if el.pdgId() == 25 and el.status() == 22 and signalBool:
                            print (" truthIt: ", truthIt)
                            print(' Higgs PDG ID = %d, Eta = %g, Phi = %g, energy [GeV] = %g, Pt [GeV] = %g, iEvt = %g, status = %g' %  
                            (el.pdgId(), el.eta(), el.phi(), el.e()/1000.0, el.pt()/1000, iEvt, el.status()))
                        truthIt += 1

                    if higgsPtCut and signalBool:
                        if any(higgs_pt > higgs_min_pt and higgs_pt < higgs_max_pt for higgs_pt in higgs_pt_values_by_event):
                            higgs_pt_values_after_cut += higgs_pt_values_by_event
                            higgs_passes_cut = True
                    
                        if not higgs_passes_cut:
                            eventCounter -= 1
                            continue
                    

                    

                    # Loop over the CaloCalTopoClusters collection
                    for el in t.CaloCalTopoClusters:
                        #print('  topo eta = %g, phi = %g, Et = %g' %  (el.eta(), el.phi(), el.et()))
                        topo_phi_values_by_event.append(el.phi())
                        topo_eta_values_by_event.append(el.eta())
                        topo_et_values_by_event.append(el.et())

                        topo_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[el.et()/1000])[0]
                        topo_log_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[np.log10(el.et() / 1000)])[0]
                        # Initialize sum of transverse energy for this event
                        topo_it += 1


                    

                        topo_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[el.et()/1000])[0]
                        topo_log_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[np.log10(el.et() / 1000)])[0]
                        # Initialize sum of transverse energy for this event
                        topo_it += 1
                    
                        

                    
                    topo_phi_values.append(topo_phi_values_by_event)
                    topo_eta_values.append(topo_eta_values_by_event)
                    topo_et_values.append(topo_et_values_by_event)
                    
                    # Loop over the CaloCalTopoClusters collection
                    topo422_it = 0
                    for el in t.CaloTopoClusters422:
                        #print('  topo422 eta = %g, phi = %g, Et = %g' %  (el.eta(), el.phi(), el.et()))
                        topo_422_phi_values_by_event.append(el.phi())
                        topo_422_eta_values_by_event.append(el.eta())
                        topo_422_et_values_by_event.append(el.et())

                        # Digitize values
                        if el.et() >= 0:
                            phi_bin = digitize(el.phi(), phi_bit_length, phi_min, phi_max)
                            eta_bin = digitize(el.eta(), eta_bit_length, eta_min, eta_max)
                            et_bin = digitize(el.et() / (et_granularity * 1000), et_bit_length, et_min / et_granularity, et_max / et_granularity)
                            #print("topo422 phi_bin, eta_bin, et_bin:", phi_bin, eta_bin, et_bin)

                            # Create binary word
                            binary_word = f"{et_bin:013b}|{eta_bin:011b}|{phi_bin:08b}"

                            # Create hex word
                            hex_word = f"{(et_bin << (eta_bit_length + phi_bit_length)) | (eta_bin << phi_bit_length) | phi_bin:08x}"

                            # Write to file
                            if topo422_it == 0:
                                f_topo.write(f"Event : {iEvt}\n")
                            f_topo.write(f"0x{topo422_it:02x} {binary_word} 0x{hex_word}\n")

                        topo_422_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[el.et()/1000])[0]
                        topo_422_log_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[np.log10(el.et() / 1000)])[0]
                        sum_et += el.et()  # Add transverse energy of each cluster
                        topo422_it += 1
                        # Initialize sum of transverse energy for this event
                    
                    topo_422_phi_values.append(topo_422_phi_values_by_event)
                    topo_422_eta_values.append(topo_422_eta_values_by_event)
                    topo_422_et_values.append(topo_422_et_values_by_event)

                    gfex_it = 0
                    for el in t.L1_gFexSRJetRoI:
                        #print('  GFEX: L1_gFexSRJetRoI eta = %g, phi = %g, Et = %g' %  (el.eta(), el.phi(), el.et()))
                        gfex_smallr_phi_values_by_event.append(el.phi())
                        gfex_smallr_eta_values_by_event.append(el.eta())
                        gfex_smallr_et_values_by_event.append(el.et())

                        if el.et() >= 0:
                            # Digitize values
                            phi_bin = digitize(el.phi(), phi_bit_length, phi_min, phi_max)
                            eta_bin = digitize(el.eta(), eta_bit_length, eta_min, eta_max)
                            et_bin = digitize(el.et() / (et_granularity * 1000), et_bit_length, et_min / et_granularity, et_max / et_granularity)
                            #print("gfex phi_bin, eta_bin, et_bin:", phi_bin, eta_bin, et_bin)

                            # Create binary word
                            binary_word = f"{et_bin:013b}|{eta_bin:011b}|{phi_bin:08b}"

                            # Create hex word
                            hex_word = f"{(et_bin << (eta_bit_length + phi_bit_length)) | (eta_bin << phi_bit_length) | phi_bin:08x}"

                            # Write to file
                            if gfex_it == 0: 
                                f_gfex.write(f"Event : {iEvt}\n")
                            f_gfex.write(f"0x{gfex_it:02x} {binary_word} 0x{hex_word}\n")

                        gfex_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[el.et()/1000])[0]
                        gfex_log_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[np.log10(el.et() / 1000)])[0]
                        gfex_it += 1

                    gfex_smallr_phi_values.append(gfex_smallr_phi_values_by_event)
                    gfex_smallr_eta_values.append(gfex_smallr_eta_values_by_event)
                    gfex_smallr_et_values.append(gfex_smallr_et_values_by_event)

                    jfex_it = 0
                    for el in t.L1_jFexSRJetRoI:
                        #print('  JFEX: L1_jFexSRJetRoI eta = %g, phi = %g, Et = %g' %  (el.eta(), el.phi(), el.et()))
                        jfex_smallr_phi_values_by_event.append(el.phi())
                        jfex_smallr_eta_values_by_event.append(el.eta())
                        jfex_smallr_et_values_by_event.append(el.et())

                        if el.et() >= 0:
                            # Digitize values
                            phi_bin = digitize(el.phi(), phi_bit_length, phi_min, phi_max)
                            eta_bin = digitize(el.eta(), eta_bit_length, eta_min, eta_max)
                            et_bin = digitize(el.et() / (et_granularity * 1000), et_bit_length, et_min / et_granularity, et_max / et_granularity)
                            #print("jfex phi_bin, eta_bin, et_bin:", phi_bin, eta_bin, et_bin)

                            # Create binary word
                            binary_word = f"{et_bin:013b}|{eta_bin:011b}|{phi_bin:08b}"

                            # Create hex word
                            hex_word = f"{(et_bin << (eta_bit_length + phi_bit_length)) | (eta_bin << phi_bit_length) | phi_bin:08x}"

                            # Write to file
                            if jfex_it == 0:
                                f_jfex.write(f"Event : {iEvt}\n")
                            f_jfex.write(f"0x{jfex_it:02x} {binary_word} 0x{hex_word}\n")

                        jfex_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[el.et()/1000])[0]
                        jfex_log_heatmap += np.histogram2d([el.eta()], [el.phi()], bins=[eta_bins, phi_bins], weights=[np.log10(el.et() / 1000)])[0]
                        jfex_it += 1
                    
                    jfex_smallr_phi_values.append(jfex_smallr_phi_values_by_event)
                    jfex_smallr_eta_values_by_event.append(jfex_smallr_eta_values_by_event)
                    jfex_smallr_et_values_by_event.append(jfex_smallr_et_values_by_event)

                    # Print the sum of transverse energy for the event
                    #print(f"  Event {iEvt}: Sum of transverse energy (ΣEt) = {sum_et/1000:.2f} GeV")
                    # Save the heatmaps
                    if signalBool:
                        plotsDir = "signalEventPlots"
                    else: 
                        plotsDir = "backgroundEventPlots"
                    if higgsAveragePt > higgs_min_pt:
                        if signalBool:
                            plot_heatmap(topo_heatmap, eta_bins, phi_bins, 'TopoClusters: $E_T$ in Eta-Phi Plane', plotsDir + f'/topo_heatmap_event{iEvt}.png', False, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(topo_422_heatmap, eta_bins, phi_bins, 'Topo422Clusters: $E_T$ in Eta-Phi Plane', plotsDir + f'/topo_422_heatmap_event{iEvt}.png', False, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(gfex_heatmap, eta_bins, phi_bins, 'gFex: $E_T$ in Eta-Phi Plane', plotsDir + f'/gfex_heatmap_event{iEvt}.png', False, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(jfex_heatmap, eta_bins, phi_bins, 'jFex: $E_T$ in Eta-Phi Plane', plotsDir + f'/jfex_heatmap_event{iEvt}.png', False, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(topo_log_heatmap, eta_bins, phi_bins, 'TopoClusters: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/topo_log_heatmap_event{iEvt}.png', True, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(topo_422_log_heatmap, eta_bins, phi_bins, 'Topo422Clusters: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/topo_422_log_heatmap_event{iEvt}.png', True, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(gfex_log_heatmap, eta_bins, phi_bins, 'gFex: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/gfex_log_heatmap_event{iEvt}.png', True, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                            plot_heatmap(jfex_log_heatmap, eta_bins, phi_bins, 'jFex: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/jfex_log_heatmap_event{iEvt}.png', True, higgs_1_pt, higgs_2_pt, higgs_1_eta, higgs_2_eta, higgs_1_phi, higgs_2_phi, True)
                        else: 
                            plot_heatmap(topo_heatmap, eta_bins, phi_bins, 'TopoClusters: $E_T$ in Eta-Phi Plane', plotsDir + f'/topo_heatmap_event{iEvt}.png')
                            plot_heatmap(topo_422_heatmap, eta_bins, phi_bins, 'Topo422Clusters: $E_T$ in Eta-Phi Plane', plotsDir + f'/topo_422_heatmap_event{iEvt}.png')
                            plot_heatmap(gfex_heatmap, eta_bins, phi_bins, 'gFex: $E_T$ in Eta-Phi Plane', plotsDir + f'/gfex_heatmap_event{iEvt}.png')
                            plot_heatmap(jfex_heatmap, eta_bins, phi_bins, 'jFex: $E_T$ in Eta-Phi Plane', plotsDir + f'/jfex_heatmap_event{iEvt}.png')
                            plot_heatmap(topo_log_heatmap, eta_bins, phi_bins, 'TopoClusters: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/topo_log_heatmap_event{iEvt}.png', True)
                            plot_heatmap(topo_422_log_heatmap, eta_bins, phi_bins, 'Topo422Clusters: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/topo_422_log_heatmap_event{iEvt}.png', True)
                            plot_heatmap(gfex_log_heatmap, eta_bins, phi_bins, 'gFex: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/gfex_log_heatmap_event{iEvt}.png', True) # FIXME add two leading jets pt here
                            plot_heatmap(jfex_log_heatmap, eta_bins, phi_bins, 'jFex: $Log(E_T)$ in Eta-Phi Plane', plotsDir + f'/jfex_log_heatmap_event{iEvt}.png', True)
                            
                        

                # Clean up the transient tree for the current file
                ROOT.xAOD.ClearTransientTrees()
                fileIt += 1
if signalBool:

    # After the loop (once you've collected all values):
    plt.hist(higgs_pt_values, bins=50, range=(0, 500), histtype='step', label='Higgs pt')
    plt.xlabel(r"$p_T$ [GeV]")
    plt.ylabel('# Higgs')
    plt.title(r"Higgs $p_T$ Distribution")
    plt.legend()

    # Save the histogram as an image
    plt.savefig('signalEventPlots/higgs_pt_distribution.png')

    if higgsPtCut:
        plt.hist(higgs_pt_values_after_cut, bins=50, range=(0, 500), histtype='step', label='Higgs pt')
        plt.xlabel(r"$p_T$ [GeV]")
        plt.ylabel('# Higgs')
        plt.title(r"Higgs $p_T$ Distribution (after cut requiring at least 1 $p_T$ > 100, < 200 GeV Higgs per event)")
        plt.legend()

        # Save the histogram as an image
        plt.savefig('signalEventPlots/higgs_pt_distribution_after_cut.png')

# After the loop (once you've collected all values):
plt.hist(jet_pt_values, bins=50, range=(0, 100), histtype='step', label='Jet pt')
plt.xlabel(r"$p_T$ [GeV]")
plt.ylabel('# Jets')
plt.title(r"Truth InTimeAntiKt4TruthJets $p_T$ Distribution")
plt.legend()
# Save the histogram as an image
plt.savefig('backgroundEventPlots/truth_jet_pt_distribution.png')

# Calculate and print min and max for phi, eta, and Et
if topo_phi_values and any(topo_phi_values):
    print("Topo Phi: min = %g, max = %g" % (
        min(min(event) for event in topo_phi_values if event), 
        max(max(event) for event in topo_phi_values if event)
    ))
else:
    print("Topo Phi: No data available.")

if topo_eta_values and any(topo_eta_values):
    print("Topo Eta: min = %g, max = %g" % (
        min(min(event) for event in topo_eta_values if event), 
        max(max(event) for event in topo_eta_values if event)
    ))
else:
    print("Topo Eta: No data available.")

if topo_et_values and any(topo_et_values):
    print("Topo Et: min = %g, max = %g" % (
        min(min(event) for event in topo_et_values if event), 
        max(max(event) for event in topo_et_values if event)
    ))
else:
    print("Topo Et: No data available.")

if topo_422_phi_values and any(topo_422_phi_values):
    print("Topo422 Phi: min = %g, max = %g" % (
        min(min(event) for event in topo_422_phi_values if event), 
        max(max(event) for event in topo_422_phi_values if event)
    ))
else:
    print("Topo422 Phi: No data available.")

if topo_422_eta_values and any(topo_422_eta_values):
    print("Topo422 Eta: min = %g, max = %g" % (
        min(min(event) for event in topo_422_eta_values if event), 
        max(max(event) for event in topo_422_eta_values if event)
    ))
else:
    print("Topo422 Eta: No data available.")

if topo_422_et_values and any(topo_422_et_values):
    print("Topo422 Et: min = %g, max = %g" % (
        min(min(event) for event in topo_422_et_values if event), 
        max(max(event) for event in topo_422_et_values if event)
    ))
else:
    print("Topo422 Et: No data available.")

if gfex_smallr_phi_values and any(gfex_smallr_phi_values):
    print("gFex Phi: min = %g, max = %g" % (
        min(min(event) for event in gfex_smallr_phi_values if event), 
        max(max(event) for event in gfex_smallr_phi_values if event)
    ))
else:
    print("gFex Phi: No data available.")

if gfex_smallr_eta_values and any(gfex_smallr_eta_values):
    print("gFex Eta: min = %g, max = %g" % (
        min(min(event) for event in gfex_smallr_eta_values if event), 
        max(max(event) for event in gfex_smallr_eta_values if event)
    ))
else:
    print("gFex Eta: No data available.")

if gfex_smallr_et_values and any(gfex_smallr_et_values):
    print("gFex Et: min = %g, max = %g" % (
        min(min(event) for event in gfex_smallr_et_values if event), 
        max(max(event) for event in gfex_smallr_et_values if event)
    ))
else:
    print("gFex Et: No data available.")

if jfex_smallr_phi_values and any(jfex_smallr_phi_values):
    print("jFex Phi: min = %g, max = %g" % (
        min(min(event) for event in jfex_smallr_phi_values if event), 
        max(max(event) for event in jfex_smallr_phi_values if event)
    ))
else:
    print("jFex Phi: No data available.")

if jfex_smallr_eta_values and any(jfex_smallr_eta_values):
    print("jFex Eta: min = %g, max = %g" % (
        min(min(event) for event in jfex_smallr_eta_values if event), 
        max(max(event) for event in jfex_smallr_eta_values if event)
    ))
else:
    print("jFex Eta: No data available.")

if jfex_smallr_et_values and any(jfex_smallr_et_values):
    print("jFex Et: min = %g, max = %g" % (
        min(min(event) for event in jfex_smallr_et_values if event), 
        max(max(event) for event in jfex_smallr_et_values if event)
    ))
else:
    print("jFex Et: No data available.")
if signalBool and higgsPtCut:
    print("percent of files passing pt cut:", eventCounter/ (fileIt * events_per_file))




#L1_jFexSRJetRoI
#L1_gFexSRJetRoI