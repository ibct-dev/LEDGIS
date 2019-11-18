#!/usr/bin/env bash
set -eo pipefail
VERSION=2.0

# Load eosio specific helper functions
. ./scripts/helpers/eosio.sh

[[ ! $NAME == "Ubuntu" ]] && set -i # Ubuntu doesn't support interactive mode since it uses dash

[[ ! -d $BUILD_DIR ]] && printf "${COLOR_RED}Please run ./led_build.sh first!${COLOR_NC}" && exit 1
echo "${COLOR_CYAN}====================================================================================="
echo "========================== ${COLOR_WHITE}Starting LED Installation${COLOR_CYAN} ==============================${COLOR_NC}"
execute cd $BUILD_DIR
execute make install
execute cd ..

echo ""
echo "    ${COLOR_RED}██╗     ███████╗██████╗  ██████╗ ██╗███████╗"
echo "    ${COLOR_RED}██║     ██╔════╝██╔══██╗██╔════╝ ██║██╔════╝"
echo "    ${COLOR_RED}██║     █████╗  ██║  ██║██║  ███╗██║███████╗"
echo "    ${COLOR_RED}██║     ██╔══╝  ██║  ██║██║   ██║██║╚════██║"
echo "    ${COLOR_RED}███████╗███████╗██████╔╝╚██████╔╝██║███████║"
echo "    ${COLOR_RED}╚══════╝╚══════╝╚═════╝  ╚═════╝ ╚═╝╚══════╝"
echo ""

echo "==============================================================================================${COLOR_NC}"
echo "${COLOR_GREEN}LED has been installed into ${EOSIO_INSTALL_DIR}/bin${COLOR_NC}"
echo "${COLOR_YELLOW}Uninstall with: ./scripts/led_uninstall.sh${COLOR_NC}"
echo "==============================================================================================${COLOR_NC}"

echo ""

echo "${COLOR_BLUE}For more information:${COLOR_NC}"
echo "${COLOR_BLUE}Ledgis website: https://ledgis.io${COLOR_NC}"
echo "${COLOR_BLUE}Ledgis medium: https://medium.com/ledgis${COLOR_NC}"
echo "${COLOR_BLUE}Ledgis Telegram channel: https://t.me/ledgis${COLOR_NC}"

echo ""
