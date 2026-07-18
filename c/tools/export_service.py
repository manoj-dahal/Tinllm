#!/usr/bin/env python3
"""
TinLLM systemd Service Exporter
Generates a linux systemd service definition file for running TinLLM as a system daemon.
"""

import os
import sys

def generate_systemd():
    current_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    exec_path = os.path.join(current_dir, "openai_server.py")

    unit_content = f"""[Unit]
Description=TinLLM MoE Inference Engine Service
After=network.target

[Service]
Type=simple
User={os.getenv('USER', 'root')}
WorkingDirectory={current_dir}
ExecStart=/usr/bin/python3 {exec_path}
Restart=always
RestartSec=3
Environment=OMP_NUM_THREADS=8
Environment=DIRECT=1
Environment=URING=1

[Install]
WantedBy=multi-user.target
"""

    out_path = os.path.join(current_dir, "tinllm.service")
    with open(out_path, "w") as f:
        f.write(unit_content)

    print(f"✓ Generated systemd unit file at: {out_path}")
    print("To install, run:")
    print("  sudo cp tinllm.service /etc/systemd/system/")
    print("  sudo systemctl daemon-reload")
    print("  sudo systemctl enable --now tinllm")

if __name__ == "__main__":
    generate_systemd()
