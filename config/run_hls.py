from pathlib import Path

# Base/default constants
base_constants = {
    "sortSeeds_": "true",
    "iterative_": "false",
    "nTotalSeeds_": 12,
    "nSeeds_": 2,
    "maxObjectsConsidered_": 1024,
    "inputEnergyCut_": 1,
    "et_granularity_": 0.25,
    "r2Cut_": 1.0,
    "et_bit_length_": 13,
    "eta_bit_length_": 11,
    "phi_bit_length_": 8,
    "phi_min_": -3.2,
    "phi_max_": 3.2,
    "eta_min_": -5.0,
    "eta_max_": 5.0,
    "et_min_": 0,
    "et_max_": 2048,
    "useMax_": "false"
}


def compute_derived(constants):
    constants["phi_range_"] = constants["phi_max_"] - constants["phi_min_"]
    return constants


def write_constants_h(constants: dict, output_file: str):
    with open(output_file, "w") as f:
        f.write("#ifndef CONSTANTS_H\n")
        f.write("#define CONSTANTS_H\n")
        f.write("// Constants used by SW & FW implementation\n")

        for key, value in constants.items():
            if isinstance(value, bool) or value in ["true", "false"]:
                f.write(f"constexpr bool {key} = {str(value).lower()};\n")
            elif isinstance(value, int):
                f.write(f"constexpr int {key} = {value};\n")
            elif isinstance(value, float):
                f.write(f"constexpr double {key} = {value};\n")
            else:
                f.write(f"constexpr auto {key} = {value};\n")

        f.write("#endif\n")


if __name__ == "__main__":
    # Sweep over combinations
    nSeeds_options = [1, 2, 4]
    r2Cut_options = [0.5, 1.0, 1.5]

    for nSeeds in nSeeds_options:
        for r2Cut in r2Cut_options:
            # Copy base constants
            constants = base_constants.copy()
            constants["nSeeds_"] = nSeeds
            constants["r2Cut_"] = r2Cut

            # Update derived values
            constants = compute_derived(constants)

            # File name based on parameter values
            file_suffix = f"nSeeds{nSeeds}_r2Cut{str(r2Cut).replace('.', '')}"
            filename = f"../algorithm/Constants_{file_suffix}.h"

            # Write to file
            write_constants_h(constants, filename)
            print(f" Wrote {filename}")
