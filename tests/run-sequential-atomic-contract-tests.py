#!/usr/bin/env python3
"""Compile the real optional contract: one valid/base-compatible case and eight targeted failures."""
import argparse
from pathlib import Path
import subprocess
import tempfile

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--system", type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    expected = {
        1: "must advertise SupportsSequentiallyConsistentLoadStore",
        2: "must support sequentially consistent loads and stores",
        3: "Sequentially consistent load must return std::uint32_t",
        4: "Sequentially consistent store must return void",
        5: "Sequentially consistent load must be noexcept",
        6: "Sequentially consistent store must be noexcept",
        7: "LoadSequentiallyConsistent",
        8: "StoreSequentiallyConsistent",
    }
    with tempfile.TemporaryDirectory(prefix="edp-sc-contract-") as temp:
        for case in range(9):
            output = str(Path(temp) / ("case-" + str(case)))
            command = [
                args.compiler, "-std=c++20", "-Wall", "-Wextra", "-Wpedantic", "-Werror",
                "-I" + str(root / "src"), "-I" + str(args.system / "src"),
                "-DEDP_PLATFORM_SC_NEGATIVE_CASE=" + str(case),
                str(root / "tests/concurrency/SequentialAtomicContractCases.cpp"), "-o", output,
            ]
            result = subprocess.run(command, text=True, capture_output=True)
            if case == 0:
                if result.returncode:
                    raise RuntimeError(result.stderr)
                subprocess.run([output], check=True)
                print("Valid optional and base-only contracts PASS")
            elif result.returncode == 0 or expected[case] not in result.stderr:
                raise RuntimeError("Negative case " + str(case) + " failed incorrectly:\n" + result.stderr)
            else:
                print("Expected optional contract rejection PASS:", case)
if __name__ == "__main__":
    main()
