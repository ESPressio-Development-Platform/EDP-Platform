#!/usr/bin/env python3
"""Compile the actual optional contract and verify targeted diagnostic rejection."""
import argparse
from pathlib import Path
import subprocess
import tempfile

def main():
    """Run one positive case and ten independently invalid provider shapes."""
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--system", required=True, type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    diagnostics = {
        1: "advertise every bounded current-context property",
        2: "satisfy every bounded current-context property",
        3: "BindCurrent must return CurrentContextBindResult",
        4: "ReadCurrent must return the exact CurrentContextReadResult",
        5: "RetireCurrent must return CurrentContextRetireResult",
        6: "BindCurrent must be noexcept",
        7: "ReadCurrent must be noexcept",
        8: "RetireCurrent must be noexcept",
        9: "unqualified object Type",
        10: "noncopyable/nonmovable",
    }
    with tempfile.TemporaryDirectory(prefix="edp-binding-contract-") as directory:
        for mode in range(11):
            output = Path(directory) / ("case-" + str(mode))
            command = [args.compiler, "-std=c++20", "-Wall", "-Wextra",
                       "-Wpedantic", "-Werror", "-I" + str(root / "src"),
                       "-I" + str(args.system / "src"),
                       "-DEDP_BINDING_NEGATIVE_CASE=" + str(mode),
                       str(root / "tests/execution/CurrentContextBindingContractCases.cpp"),
                       "-o", str(output)]
            result = subprocess.run(command, capture_output=True, text=True, timeout=60)
            if mode == 0:
                if result.returncode:
                    raise RuntimeError(result.stderr)
                subprocess.run([str(output)], check=True, timeout=10)
            elif result.returncode == 0 or diagnostics[mode] not in result.stderr:
                raise RuntimeError("Wrong rejection for mode " + str(mode) + "\n" + result.stderr)
            print("PASS binding contract case", mode)

if __name__ == "__main__":
    main()
