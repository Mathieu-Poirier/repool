import os

SRC_DIRS = ["src", "headers"]
TEST_DIR = "tests"

os.makedirs(TEST_DIR, exist_ok=True)

def generate_test_file(name, include_path):
    test_filename = f"{name}_test.cpp"
    test_path = os.path.join(TEST_DIR, test_filename)

    if not os.path.exists(test_path):
        with open(test_path, "w") as f:
            f.write(f"""// Auto-generated test for {name}
#include <iostream>
#include <cassert>
#include "../{include_path}/{name}.hpp"  // Adjust if needed

int main() {{
    std::cout << "Running tests for {name}..." << std::endl;
    // TODO: Add tests
    return 0;
}}
""")
        print(f"[+] Created: {test_path}")
    else:
        print(f"[=] Already exists: {test_path}")

for directory in SRC_DIRS:
    if not os.path.exists(directory):
        continue
    for filename in os.listdir(directory):
        if filename.endswith(".cpp") or filename.endswith(".hpp"):
            base = os.path.splitext(filename)[0]
            generate_test_file(base, directory)

