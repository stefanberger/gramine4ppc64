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

/* Find an entry starting with a word in the cpuinfo description.
 * This function will return a pointer to the string at the position
 * after the ': ' found in that line.
 *
 * @word: Word to look for
 * @buf: NULL terimated buffer to search for word
 */
static const char* find_entry_in_cpuinfo(const char* const word, const char* const buf) {
    const size_t word_size = strlen(word);
    size_t size = strlen(buf);
    size_t i = 0,
           j;

    if (word_size > size)
        return NULL;

    while (i < size - word_size && buf[i]) {
        j = 0;
        while (j < word_size && buf[i + j] == word[j])
            j++;

        if (j)
            i += j;
        else
            i += 1;

        if (j < word_size)
            continue;

        /* search for ': ' in this line and return pointer after this */
        while ((i < size) && (buf[i] == ' ' || buf[i] == '\t'))
            i++;
        if (i == size || (buf[i] != ':' && buf[i + 1] != ' '))
            return NULL;

        return &buf[i + 2];
    }

    return NULL;
}

double get_bogomips_from_cpuinfo_buf(const char* buf, size_t size) {
    /* We could use strstr if graphene had one. */
    /* Each prefix of the word "bogomips" occurs only once in the whole word, hence this works. */
    const char* const word = "bogomips";
    const size_t word_size = strlen(word);
    size_t i = 0,
           j = 0;

    if (word_size > size) {
        return 0.0;
    }

    while (i < size - word_size && buf[i]) {
        j = 0;
        while (j < word_size && buf[i + j] == word[j]) {
            j++;
        }

        if (j) {
            i += j;
        } else {
            i += 1;
        }

        if (j == word_size) {
            /* buf is null-terminated, so no need to check size. word does not contain neither
             * spaces nor tabs, hence we can forward global index `i`, even if we do not return
             * here. */
            while (buf[i] == ' ' || buf[i] == '\t') {
                i++;
            }
            if (buf[i] == ':') {
                return proc_cpuinfo_atod(&buf[i + 1]);
            }
        }
    }

    return 0.0;
}

double sanitize_bogomips_value(double v) {
    if (!__builtin_isnormal(v) || v < 0.0) {
        return 0.0;
    }
    return v;
}

char* get_cpu_from_cpuinfo_buf(const char* buf) {
    const char* entry = find_entry_in_cpuinfo("cpu", buf);
    if (!entry)
        return NULL;

    size_t len = 0;
    while (entry[len] != 0 && entry[len] != '\n')
        len++;

    char* res = malloc(len + 1);
    if (!res)
        return NULL;

    memcpy(res, entry, len);
    res[len] = 0;

    return res;
}
