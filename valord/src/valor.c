#include "process.h"
#include "aassert.h"

#include <valor/db/db.h>
#include <valor/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <android/log.h>

#define DB_FILE "/system/etc/valor.db"
#define IDLE_TIME 3

int main(void) {
    __android_log_print(ANDROID_LOG_INFO, MODNAME, "starting up...");
    database_t *db = db_init(DB_SIZE);
    FILE *file = fopen(DB_FILE, "r");
    if (!file) {
        cerror("fopen");
        __android_log_print(ANDROID_LOG_FATAL, MODNAME, "Failed to open %s for reading!", DB_FILE);
        return -1;
    }
    db_read(db, file);
    fclose(file);
    __android_log_print(ANDROID_LOG_INFO, MODNAME, "loaded database_t from %s", DB_FILE);

    size_t i;

    for (;;) {
        proccess_array_t *processes = get_processes();
#if DEBUG
        __android_log_print(ANDROID_LOG_DEBUG, MODNAME, "Found %zu processes", processes->length);
#endif
        for (i = 0; i < processes->length; ++i) {
            db_process_entry_t *entry = db_get_process_entry(db, processes->processes[i].checksum);
#if DEBUG
            __android_log_print(ANDROID_LOG_DEBUG, MODNAME, "PID=%d, chksum=%d", processes->processes[i].pid,
                                processes->processes[i].checksum);
#endif
            if (entry) {
                __android_log_print(ANDROID_LOG_WARN, MODNAME, "detected threat %s, killing immediatly",
                                    db->names[entry->id]);
                if (kill(processes->processes[i].pid, SIGKILL)) {
                    __android_log_print(ANDROID_LOG_ERROR, MODNAME, "Failed to kill %d: kill: %s(%d)",
                                        processes->processes[i].pid, strerror(errno), errno);
                }
            }
        }
        free_process_array(processes);
        sleep(IDLE_TIME);
    }
}