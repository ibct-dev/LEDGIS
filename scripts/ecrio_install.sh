#!/usr/bin/env bash
set -eo pipefail
VERSION=2.0

# Load eosio specific helper functions
. ./scripts/helpers/eosio.sh

[[ ! $NAME == "Ubuntu" ]] && set -i # Ubuntu doesn't support interactive mode since it uses dash

[[ ! -d $BUILD_DIR ]] && printf "${COLOR_RED}Please run ./ecrio_build.sh first!${COLOR_NC}" && exit 1
echo "${COLOR_CYAN}====================================================================================="
echo "========================== ${COLOR_WHITE}Starting ECRIO Installation${COLOR_CYAN} ==============================${COLOR_NC}"
execute cd $BUILD_DIR
execute make install
execute cd ..

echo ""
echo "    ${COLOR_RED}███████╗ ██████╗██████╗ ██╗ ██████╗ \n"
echo "    ${COLOR_RED}██╔════╝██╔════╝██╔══██╗██║██╔═══██╗\n"
echo "    ${COLOR_RED}█████╗  ██║     ██████╔╝██║██║   ██║\n"
echo "    ${COLOR_RED}██╔══╝  ██║     ██╔══██╗██║██║   ██║\n"
echo "    ${COLOR_RED}███████╗╚██████╗██║  ██║██║╚██████╔╝\n"
echo "    ${COLOR_RED}╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝ ╚═════╝ \n"
echo ""

echo "==============================================================================================${COLOR_NC}"
echo "${COLOR_GREEN}ECRIO has been installed into ${EOSIO_INSTALL_DIR}/bin${COLOR_NC}"
echo "\\n${COLOR_YELLOW}Uninstall with: ./scripts/ecrio_uninstall.sh${COLOR_NC}\\n"
echo "==============================================================================================${COLOR_NC}"
echo ""

echo "${COLOR_BLUE}For more information:\\n"
echo "${COLOR_BLUE}EOS Chrome website: https://kr.eoschrome.io\\n"
echo "${COLOR_BLUE}EOS Chrome medium: https://medium.com/eoschrome\\n"
echo "${COLOR_BLUE}EOS Chrome Telegram channel: https://t.me/eos_chrome\\n"
resources