#!/bin/bash
# 用法：./read_config.sh config.ini database host
CONFIG_FILE="$1"
SECTION="$2"
KEY="$3"

# 使用 grep + awk 提取值
awk -F '=' '/\['"$SECTION"'\]/{found=1} found && $1~/'"$KEY"'/{print $2; exit}' "$CONFIG_FILE"
