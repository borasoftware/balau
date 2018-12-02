#!/bin/bash

PROJECT_VERSION="${1}"

# Executed in the build folder.

cd ../src/doc/manual
RELATIVE_PATHS=$(find -type f -name '*.bdml' | sed "s|^\./||")
cd -

mkdir -p ${PROJECT_VERSION}/manual

cd ${PROJECT_VERSION}/manual

cp -R ../../../src/doc/bdml ..
cp -R ../../../src/doc/resources ..

BDML_HTML_TRANSFORM="../../../src/doc/bdml/BdmlHtml.xsl"

for RELATIVE_PATH in ${RELATIVE_PATHS}; do
	BDML_FILE="../../../src/doc/manual/${RELATIVE_PATH}"
	ROOT_PATH=$(dirname "${RELATIVE_PATH}")
	NAME_ROOT=$(basename "${RELATIVE_PATH}" .bdml)
	HTML_FILE="${ROOT_PATH}/${NAME_ROOT}.html"

	if [ ! -z "${BASE_PATH}" ] ; then
		mkdir -p "${BASE_PATH}"
	fi

	echo "Translating ${BDML_FILE} to ${HTML_FILE}"
	xsltproc --stringparam no-link-conversion false -o "${HTML_FILE}" "${BDML_HTML_TRANSFORM}" "${BDML_FILE}"
done
