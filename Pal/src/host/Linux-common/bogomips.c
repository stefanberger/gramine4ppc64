#include "api.h"
#include "bogomips.h"

/* This version is too dumb to be shared by the whole repository and should be removed once we get
 * a proper stdlib (like musl). */
static double proc_cpuinfo_atod(const char* s) {
    double ret = 0.0;
    char* end = NULL;
    double base, fractional;

    base = strtol(s, &end, 10);

    if (*end == '.') {
        s = end + 1;
        fractional = strtol(s, &end, 10);
        while (s != end) {
            fractional /= 10.0;
            s++;
        }
        ret = base + fractional;
    }

    return ret;
}

/* Find an entry starting with a `word` in the NULL-terminated `cpuinfo` description.
 * This function will return a pointer to the string at the position after the ': '
 * found in that line, NULL otherwise.
 */
static char* find_entry_in_cpuinfo(const char* cpuinfo, const char* word) {
    char* start = strstr(cpuinfo, word);
    if (!start)
        return NULL;

    unsigned int o = strlen(word);
    while (start[o] && (start[o] == '\t' || start[o] == ' '))
        o++;

    if (start[o] == ':' && start[o + 1] == ' ')
        return &start[o + 2];

    return NULL;
}

double get_bogomips_from_cpuinfo_buf(const char* buf) {
    char* start = find_entry_in_cpuinfo(buf, "bogomips");
    if (!start)
        return 0.0;
    return proc_cpuinfo_atod(start);
}

double sanitize_bogomips_value(double v) {
    if (!__builtin_isnormal(v) || v < 0.0) {
        return 0.0;
    }
    return v;
}

char* get_string_from_cpuinfo_buf(const char* cpuinfo, const char *entry) {
    const char* line = find_entry_in_cpuinfo(entry, cpuinfo);
    if (!line)
        return NULL;

    size_t len = 0;
    while (line[len] != 0 && line[len] != '\n')
        len++;

    char* res = malloc(len + 1);
    if (!res)
        return NULL;

    memcpy(res, line, len);
    res[len] = 0;

    return res;
}
