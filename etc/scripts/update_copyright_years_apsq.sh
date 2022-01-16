#!/bin/bash

# SPDX-FileCopyrightText: 2022 CERN and the Allpix Squared authors
#
# SPDX-License-Identifier: MIT

set -e

UPDATE_SCRIPT=$(dirname $0)/update_copyright_years.py
APPENDIX="CERN and the Allpix Squared authors"

python3 ${UPDATE_SCRIPT} -v -a "${APPENDIX}" -x 3rdparty/

# update copyright info output allpix exe
python3 ${UPDATE_SCRIPT} -v -a "${APPENDIX}" -p "std::cout << \"Copyright (c)" -f src/exec/allpix.cpp
