#!/bin/bash

SOURCE_DIRS="/sciezka/do/1pliku /sciezka/do/2pliku"
BACKUP_DIR="/backup"
LOG_FILE="/var/log/backup.log"
DATE=$(date '+%Y-%m-%d')

BACKUP_FILE="${BACKUP_DIR}/backup_${DATE}.tar.gz"

echo "$(date '+%Y-%m-%d %H:%M:%S') - tworzenie kopii zapasowej" >> "$LOG_FILE"
if tar -czf "$BACKUP_FILE" $SOURCE_DIRS 2>>"$LOG_FILE"; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - kopia zapasowa utworzona plik: $BACKUP_FILE" >> "$LOG_FILE"
else
    echo "$(date '+%Y-%m-%d %H:%M:%S') - błąd podczas tworzenia kopii zapasowej" >> "$LOG_FILE"
fi

find "$BACKUP_DIR" -type f -name "backup_*.tar.gz" -mtime +7 -exec rm {} \;
if [ $? -eq 0 ]; then
    echo "$(date '+%Y-%m-%d %H:%M:%S') - kopie zapasowe starsze niż 7 dni usunięte." >> "$LOG_FILE"
fi

exit 0
