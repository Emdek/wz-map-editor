#!/bin/sh

export PATH=/sw/lib/qt4-mac/bin:/sw/lib/qt4-x11/bin:/opt/local/bin:${PATH}
uic -o "${DERIVED_FILES_DIR}/ui_${INPUT_FILE_BASE}.h" "${INPUT_FILE_PATH}" || exit 1
moc -nw -o "${DERIVED_FILES_DIR}/${INPUT_FILE_BASE}.ui.moc.cpp" "${DERIVED_FILES_DIR}/ui_${INPUT_FILE_BASE}.h"
exit 0
