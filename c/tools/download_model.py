#!/usr/bin/env python3
"""
TinLLM Model Download Utility
Downloads pre-quantized GLM-5.2 / DeepSeek int4 model shards from Hugging Face repository mirrors.
"""

import os
import sys
import argparse

DEFAULT_REPO = "mateogrgic/GLM-5.2-tinllm-int4-with-int8-mtp"

def download_model():
    parser = argparse.ArgumentParser(description="TinLLM HuggingFace Model Downloader")
    parser.add_argument("--repo", type=str, default=DEFAULT_REPO, help="Hugging Face repository ID")
    parser.add_argument("--dest", type=str, default="/home/user/models/glm52_int4", help="Destination path")
    args = parser.parse_args()

    print(f"[TinLLM Downloader] Repository: {args.repo}")
    print(f"[TinLLM Downloader] Destination: {args.dest}")

    os.makedirs(args.dest, exist_ok=True)

    try:
        from huggingface_hub import snapshot_download
        print("Downloading repository shards via huggingface_hub...")
        snapshot_download(repo_id=args.repo, local_dir=args.dest, resume_download=True)
        print("✓ Download completed successfully!")
    except ImportError:
        print("⚠️ 'huggingface_hub' package not found.")
        print(f"To install and download, run: pip install huggingface_hub\nOr download manually from: https://huggingface.co/{args.repo}")

if __name__ == "__main__":
    download_model()
