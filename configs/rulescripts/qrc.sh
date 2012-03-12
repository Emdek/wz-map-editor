#!/bin/sh

export PATH=/sw/lib/qt4-mac/bin:/sw/lib/qt4-x11/bin:/opt/local/bin:${PATH}
rcc -o "${DERIVED_FILES_DIR}/${INPUT_FILE_BASE}.rcc.cpp" "${INPUT_FILE_PATH}" || exit 1
exit 0
