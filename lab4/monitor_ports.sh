#!/bin/bash

LOG_FILE="/var/log/open_ports.log"
SLEEP_INTERVAL=60

monitor_ports() {
    echo "[$(date)] - Monitorowanie portów" >> "$LOG_FILE"
    ss -tuln >> "$LOG_FILE"
    echo "########################################" >> "$LOG_FILE"
}

while true; do
    monitor_ports
    sleep "$SLEEP_INTERVAL"
done
