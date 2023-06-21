#!/bin/sh
cd "${0%/*}" || exit 1

[ -d venv ] || python3 -m venv venv
. venv/bin/activate
pip install -U pip
pip install -r requirements.txt

pytest
