#include "fcntl.h"
#include "ctype.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

typedef struct Str Str;

struct Str
{
    char *ptr;
    long len;
    long real_len;
};

Str *init_str(void)
{
    Str *str = malloc(sizeof *str);
    str->len = 0;
    str->real_len = 10;
    str->ptr = malloc(str->real_len);
    str->ptr[0] = '\0';
    return str;
}

void delete_str(Str *str)
{
    if (str == NULL) {
        return;
    }
    if (str->ptr != NULL) {
        free(str->ptr);
    }
    free(str);
}

void add_sym(Str *str, char sym)
{
    if (str->len + 1 >= str->real_len) {
        str->real_len *= 2;
        str->ptr = realloc(str->ptr, str->real_len);
    }
    str->ptr[str->len++] = sym;
    str->ptr[str->len] = '\0';
}

enum { BUF_SIZE = 512 };

char *buf = NULL;
long ptr = 0;
long len = 0;

char *sep;
long flag = 0;
long match = 0;
long sep_len = 0;

Str *max_val = NULL;
Str *val = NULL;

void my_exit(void)
{
    if (max_val != NULL) {
        delete_str(max_val);
        max_val = NULL;
    }
    if (val != NULL) {
        delete_str(val);
        val = NULL;
    }
    if (buf != NULL) {
        free(buf - sep_len);
        buf = NULL;
    }
    exit(1);
}

void update_buffer(int fd)
{
    if (ptr >= len) {
        ptr = 0;
        len = read(fd, buf, BUF_SIZE);

        if (len == -1) {
            my_exit();
        }
    }
}

void process_sep(int fd)
{
    update_buffer(fd);

    match = 0;

    while (1) {
        if (buf[ptr] == sep[match]) {
            match++;
        } else {
            flag = 0;
            if (match > 0) {
                ptr -= match;
                if (ptr < 0) {
                    memcpy(buf + ptr, sep, match);
                }
                match = 0;
            }
            return;
        }

        ptr++;
        update_buffer(fd);

        if (match == sep_len) {
            flag = 1;
            match = 0;
            return;
        }
    }
}

int is_prev_zero(Str *str)
{
    long count = 0;

    while (count < str->len && str->ptr[count] == '0') {
        count++;
    }

    if (count == 0) {
        return 0;
    }

    if (str->len == 1) {
        return 0;
    }

    return 1;
}

void get_val(Str *str, int fd)
{
    str->len = 0;
    str->ptr[0] = '\0';

    update_buffer(fd);

    if (len <= 0) {
        return;
    }

    while (len > 0 && isdigit(buf[ptr])) {
        if (buf[ptr] == sep[0]) {
            process_sep(fd);

            if (flag == 1) {
                return;
            }
        }

        add_sym(str, buf[ptr]);
        ptr++;
        update_buffer(fd);
    }
}

int cmp_val(Str *a, Str *b)
{
    if (a->len == b->len) {
        return strcmp(a->ptr, b->ptr);
    } else if (a->len > b->len) {
        return 1;
    } else {
        return -1;
    }
}

void write_str(Str *str, int fd)
{
    add_sym(str, '\n');

    long code = write(fd, str->ptr, str->len);

    if (code <= 0) {
        my_exit();
    }
}

void process_line(int fd)
{
    get_val(max_val, fd);

    if (is_prev_zero(max_val)) {
        my_exit();
    }

    if (max_val->len == 0) {
        return;
    }

    while (1) {
        if (flag == 1) {
            flag = 0;
        } else {
            process_sep(fd);

            if (flag == 0) {
                break;
            } else {
                flag = 0;
            }
        }

        get_val(val, fd);

        if (val->len <= 0) {
            my_exit();
        }

        if (is_prev_zero(val)) {
            my_exit();
        }

        if (cmp_val(val, max_val) > 0) {
            Str *swap = max_val;
            max_val = val;
            val = swap;
        }
    }
}

int temp_file(void)
{
    char s[] = "tmpXXXXXX";

    int fd = mkstemp(s);

    if (fd == -1) {
        my_exit();
    }

    unlink(s);

    return fd;
}

void process(int reader, int writer)
{
    max_val = init_str();
    val = init_str();

    int line_counter = 0;

    do {
        process_line(reader);

        update_buffer(reader);

        if (len == 0) {
            if (max_val->len == 0 && line_counter == 0) {
                return;
            }
            my_exit();
        }

        if (max_val->len == 0) {
            my_exit();
        }

        if (buf[ptr] != '\n') {
            my_exit();
        }

        line_counter++;

        ptr++;
        update_buffer(reader);

        write_str(max_val, writer);
    } while (len > 0);

    delete_str(val);
    delete_str(max_val);
    max_val = NULL;
    val = NULL;
}

void copy(int reader, int writer)
{
    while (1) {
        len = read(reader, buf, BUF_SIZE);
        if (len == -1) {
            my_exit();
        }

        if (len <= 0) {
            return;
        }

        long code = write(writer, buf, len);
        if (code == -1) {
            my_exit();
        }
    }
}

int main(int argc, char **argv)
{
    // read arguments

    char *file_name;

    char default_sep[] = ",";

    if (argc == 2) {
        sep = default_sep;
    } else if (argc == 3) {
        sep = argv[2];
    } else {
        return 1;
    }

    sep_len = strlen(sep);

    file_name = argv[1];

    buf = malloc(BUF_SIZE + sep_len);
    buf += sep_len;

    int temp = temp_file();

    int reader = open(file_name, O_RDONLY);
    if (reader == -1) {
        return 1;
    }

    process(reader, temp);

    close(reader);

    int main_writer = open(file_name, O_WRONLY | O_TRUNC);
    if (main_writer == -1) {
        exit(1);
    }

    lseek(temp, 0, SEEK_SET);

    copy(temp, main_writer);

    close(temp);
    close(main_writer);

    free(buf - sep_len);
    buf = NULL;

    return 0;
}
