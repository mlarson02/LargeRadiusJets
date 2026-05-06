import uproot
import awkward as ak
import pandas as pd
import sys

input_file  = sys.argv[1] if len(sys.argv) > 1 else "ntuple.root"
output_file = sys.argv[2] if len(sys.argv) > 2 else "jetTagger.parquet"

f = uproot.open(input_file)

lead     = f["jetTaggerLeadingLRJsTree"]
sublead  = f["jetTaggerSubleadingLRJsTree"]
evtinfo  = f["eventInfoTree"]
conejets = f["gepWTAConeCellsTowersSKJetsTree"]

def first(tree, branch):
    """Return a flat numpy array — takes first element if vector branch, otherwise flat scalar."""
    arr = tree[branch].array()
    if arr.ndim == 1:
        return ak.to_numpy(arr)
    return ak.to_numpy(arr[:, 0])

def jagged(tree, branch):
    """Return a list-of-lists for variable-length branches (one list per event)."""
    return ak.to_list(tree[branch].array())

df = pd.DataFrame({
    # --- Leading LRJ ---
    "jetTaggerLeadingLRJEt":                  first(lead, "Et"),
    "jetTaggerLeadingLRJEta":                 first(lead, "Eta"),
    "jetTaggerLeadingLRJPhi":                 first(lead, "Phi"),
    "jetTaggerLeadingLRJMassApprox":          first(lead, "MassApprox"),
    "jetTaggerLeadingLRJTau_1":               first(lead, "Tau_1"),
    "jetTaggerLeadingLRJTau_2":               first(lead, "Tau_2"),
    "jetTaggerLeadingLRJTau_21":              first(lead, "Tau_21"),
    "jetTaggerLeadingLRJPsi_R":               first(lead, "Psi_R"),
    "jetTaggerLeadingLRJSubjetMultiplicity":  first(lead, "SubjetMultiplicity"),

    # --- Subleading LRJ ---
    "jetTaggerSubleadingLRJEt":                  first(sublead, "Et"),
    "jetTaggerSubleadingLRJEta":                 first(sublead, "Eta"),
    "jetTaggerSubleadingLRJPhi":                 first(sublead, "Phi"),
    "jetTaggerSubleadingLRJMassApprox":          first(sublead, "MassApprox"),
    "jetTaggerSubleadingLRJTau_1":               first(sublead, "Tau_1"),
    "jetTaggerSubleadingLRJTau_2":               first(sublead, "Tau_2"),
    "jetTaggerSubleadingLRJTau_21":              first(sublead, "Tau_21"),
    "jetTaggerSubleadingLRJPsi_R":               first(sublead, "Psi_R"),
    "jetTaggerSubleadingLRJSubjetMultiplicity":  first(sublead, "SubjetMultiplicity"),

    # --- SK WTA cone jets (variable multiplicity per event) ---
    "coneJetSK_pT":            jagged(conejets, "pT"),
    "coneJetSK_Eta":           jagged(conejets, "Eta"),
    "coneJetSK_Phi":           jagged(conejets, "Phi"),
    "coneJetSK_NConstituents": jagged(conejets, "NConstituents"),

    # --- Event info ---
    "mcEventWeight":          first(evtinfo, "mcEventWeight"),
    "sumOfWeightsForSample":  first(evtinfo, "sumOfWeightsForSample"),
    "eventWeights":           first(evtinfo, "eventWeights"),
    "sampleJZSlice":          first(evtinfo, "sampleJZSlice"),
})

df.to_parquet(output_file, index=False)
print(f"Wrote {len(df)} events to {output_file}")
