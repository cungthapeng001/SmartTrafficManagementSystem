#!/bin/bash
# Smart Traffic Management System - Launcher

cd "$(dirname "$0")"
source ui/venv/bin/activate
python3 ui/main.py
