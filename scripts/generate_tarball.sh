#!/bin/bash
set -eo pipefail

NAME="$1"
EOS_PREFIX="${PREFIX}/${SUBPREFIX}"
mkdir -p "${PREFIX}/bin/"
mkdir -p "${EOS_PREFIX}/bin"
mkdir -p "${EOS_PREFIX}/licenses/ledgis"
mkdir -p "${EOS_PREFIX}/etc/ledgis/contracts"

# install binaries 
cp -R "${BUILD_DIR}"/bin/* ${EOS_PREFIX}/bin

# install licenses
cp -R "${BUILD_DIR}"/licenses/ledgis/* ${EOS_PREFIX}/licenses

# install bios and boot contracts
cp -R "${BUILD_DIR}"/contracts/contracts/led.bios/led.bios.* ${EOS_PREFIX}/etc/ledgis/contracts
cp -R "${BUILD_DIR}"/contracts/contracts/led.boot/led.boot.* ${EOS_PREFIX}/etc/ledgis/contracts

for f in $(ls "${BUILD_DIR}/bin/"); do
    bn=$(basename "$f")
    ln -sf ../"${SUBPREFIX}/bin/$bn" "${PREFIX}/bin/$bn"
done
echo "Compressing '$NAME.tar.gz'..."
tar -cvzf "$NAME.tar.gz" ./"${PREFIX}"/*
rm -r "${PREFIX}"
