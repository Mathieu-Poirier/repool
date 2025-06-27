import os

SRC_DIR = "src"
HEADER_DIR = "headers"

os.makedirs(HEADER_DIR, exist_ok=True)

for filename in os.listdir(SRC_DIR):
    if filename.endswith(".cpp"):
        base = os.path.splitext(filename)[0]
        header_filename = f"{base}.hpp"
        header_path = os.path.join(HEADER_DIR, header_filename)

        if not os.path.exists(header_path):
            with open(header_path, "w") as f:
                f.write(f"""#pragma once

// Declarations for {base}.cpp

""")
            print(f"[+] Created: {header_path}")
        else:
            print(f"[=] Already exists: {header_path}")
