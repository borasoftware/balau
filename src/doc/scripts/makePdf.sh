#!/bin/bash

PROJECT_VERSION="${1}"

# Executed in the build folder.

mkdir -p ${PROJECT_VERSION}/pdf
cd ${PROJECT_VERSION}/pdf

OUTPUT_FILE="$(pwd)/balau-manual.pdf"

cd ../../../src/doc/single-page

BDML_FILE="balau-manual.bdml"

echo "Running in $(pwd)"
echo "Translating ${BDML_FILE} to ${OUTPUT_FILE}"
bdml.sh output=${OUTPUT_FILE} ${BDML_FILE}
