#ifndef WHEXD_H
#define WHEXD_H

#define VERSION "v0.1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define __WHEXD_PRINTABLE_CHARS_START       0x20
#define __WHEXD_PRINTABLE_CHARS_END         0x7f
#define __WHEXD_CONTROL_CHAR_REPLACEMENT    0x2e    // Dot "."

// Print modes
#define __WHEXD_MODE_TWO_BYTE_HEX   0x00    // -x, --two-bytes-hex (Default)
#define __WHEXD_MODE_ONE_BYTE_HEX   0x01    // -X, --one-byte-hex
#define __WHEXD_MODE_TWO_BYTE_OCT   0x02    // -o, --two-bytes-octal
#define __WHEXD_MODE_ONE_BYTE_OCT   0x03    // -b, --one-byte-octal
#define __WHEXD_MODE_TWO_BYTE_DEC   0x04    // -d, --two-bytes-decimal
#define __WHEXD_MODE_ONE_BYTE_CHR   0x05    // -c, --one-byte-char
#define __WHEXD_MODE_CANONICAL      0x06    // -C, --canonical

// Formats
#define __WHEXD_FMT_ADDR_07x        "%07x"  // Default address print format
#define __WHEXD_FMT_ADDR_08x        "%08x"  // Only used in Canonical print mode
#define __WHEXD_FMT_TWO_BYTE_HEX    "%04x"
#define __WHEXD_FMT_ONE_BYTE_HEX    "%02x"
#define __WHEXD_FMT_ONE_BYTE_OCT    "%03o"
#define __WHEXD_FMT_TWO_BYTE_OCT    " %06o"
#define __WHEXD_FMT_TWO_BYTE_DEC    "  %05u"
#define __WHEXD_FMT_ONE_BYTE_CHR    "%c"

#define __WHEXD_FMT_ADDR_07x_ID 0x01
#define __WHEXD_FMT_ADDR_08x_ID 0x02

// Read 16 bytes at a time
#define __WHEXD_MAX_BYTES_READ  16U

typedef struct __whexd_mode {
    u8   print_mode;
    u8   verbose;
    u32  skip_bytes;
    u32  length;
    char addr_fmt[5];
    char byte_fmt[7];
    u8   base;
} __whexd_mode;

void __whexd_init_mode(__whexd_mode *mode)
{
    mode->print_mode = __WHEXD_MODE_TWO_BYTE_HEX;
    mode->verbose    = 0;
    mode->length     = UINT32_MAX;
    mode->skip_bytes = 0;
    strcpy_s(mode->addr_fmt, 10U, __WHEXD_FMT_ADDR_07x);
    strcpy_s(mode->byte_fmt, 10U, __WHEXD_FMT_TWO_BYTE_HEX);
    mode->base       = 16;
}

static u8 __whexd_buf[__WHEXD_MAX_BYTES_READ] = {0},
          __whexd_prev[__WHEXD_MAX_BYTES_READ] = {0};

// Flag that indicates if the current bytes are identical to the
// previously printed
static u8 __whexd_identical_line = 0;

inline void __whexd_print_address(size_t addr, const char *fmt) { printf(fmt, addr); }
inline void __whexd_newline(void) { printf("%s", "\n"); }

void __whexd_print_help(void);
void __whexd_print_version(void);

inline void __whexd_print_bytes(size_t bytes_read, u8 step, const char *fmt)
{
    u16 bytes;
    for (u8 i = 0; i < bytes_read; i += step)
    {
        printf(" ");

        bytes = *((u16*)(__whexd_buf + i));
        printf(fmt, bytes - (bytes & 0xff00) * (step & 0x01));
    }
    __whexd_newline();
}

inline void __whexd_print_decimal(size_t bytes_read, u8 step, const char *fmt)
{
    u16 num;
    for (u8 i = 0; i < bytes_read; i += step)
    {
        printf(" ");

        num = *((u16*)(__whexd_buf + i));
        printf(fmt, num - (num / 100) * (step & 1) * 100);
    }
    __whexd_newline();
}

inline void __whexd_print_ascii(size_t bytes_read)
{
    printf("%s", " |");
    for (u8 i = 0; i < bytes_read; ++i)
    {
        if (__whexd_buf[i] >= __WHEXD_PRINTABLE_CHARS_START &&
            __whexd_buf[i] <= __WHEXD_PRINTABLE_CHARS_END)
            printf("%c", __whexd_buf[i]);
        else
            printf("%c", __WHEXD_CONTROL_CHAR_REPLACEMENT);
    }
    printf("%s", "|");
}

char __whexd_parse_args(int argc, char *argv[], __whexd_mode *mode, char *filename)
{
    if (mode == NULL || filename == NULL)
        return -2;

    for(u8 i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-n") == 0)
        {
            uint32_t res = strtoul(argv[i + 1], NULL, 10);

            if (res == 0)
            {
                res = strtoul(argv[i + 1], NULL, 16);

                if (res == 0)
                {
                    fprintf(stderr, "whexd: failed to parse length: '%s': Invalid argument\n", argv[i + 1]);
                    return -1;
                }
            }

            mode->length = res;
            i += 1;
        }
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--skip") == 0)
        {
            uint32_t skip = strtoul(argv[i + 1], NULL, 10);

            if (skip == 0)
            {
                skip = strtoul(argv[i + 1], NULL, 16);

                if (skip == 0)
                {
                    fprintf(stderr, "whexd: failed to parse offset: '%s': Invalid argument\n", argv[i + 1]);
                    return -1;
                }
            }

            mode->skip_bytes = skip;
            i += 1;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--no-squeezing") == 0)
        {
            mode->verbose = 1;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            __whexd_print_help();
        }
        else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--version") == 0)
        {
            __whexd_print_version();
        }
        else if (strchr(argv[i], '-') != NULL)
        {
            fprintf(stderr, "whexd: invalid option -- '%s'\n", argv[i]);
            return -1;
        }
        else
        {
            FILE *test;
            fopen_s(&test, argv[i], "r");

            if (test == NULL)
            {
                fprintf(stderr, "whexd: %s: No such file or directory\n", argv[i]);
                return -1;
            }
            
            fclose(test);
            strcpy_s(filename, _MAX_PATH, argv[i]);
        }
    }

    return 1;
}

char whexdump(int argc, char *argv[])
{    
    __whexd_mode mode;
    __whexd_init_mode(&mode);

    char filename[_MAX_PATH];
    if (__whexd_parse_args(argc, argv, &mode, &filename[0]) != 1)
        return -1;

    FILE *file;
    fopen_s(&file, filename, "rb");

    if (file == NULL)
        return -1;

    const u8 byte_step = 2 - (mode.print_mode & 0x01);
    size_t bytes_read = 0, addr = 0x00, byte_count = 0;

    addr += mode.skip_bytes;
    fseek(file, mode.skip_bytes, SEEK_SET);

    bytes_read = fread(__whexd_buf, 1U, __WHEXD_MAX_BYTES_READ, file);
    while (bytes_read == __WHEXD_MAX_BYTES_READ && byte_count < mode.length)
    {
        if (!mode.verbose &&
            !memcmp(__whexd_buf, __whexd_prev, __WHEXD_MAX_BYTES_READ))
        {
            if (!__whexd_identical_line)
                printf("*\n");
            __whexd_identical_line = 1;
        }
        else
        {
            __whexd_print_address(addr, mode.addr_fmt);
            __whexd_print_bytes(__WHEXD_MAX_BYTES_READ, byte_step, mode.byte_fmt);

            memcpy(__whexd_prev, __whexd_buf, __WHEXD_MAX_BYTES_READ);
            __whexd_identical_line = 0;
        }

        addr += 0x10;
        byte_count += __WHEXD_MAX_BYTES_READ;

        bytes_read = fread(__whexd_buf, 1U, __WHEXD_MAX_BYTES_READ, file);
    }
    fclose(file);

    // Print residual bytes, if any
    if (bytes_read != 0)
    {
        __whexd_buf[bytes_read] = 0x00;

        __whexd_print_address(addr, mode.addr_fmt);
        __whexd_print_bytes(bytes_read, byte_step, mode.byte_fmt);
    }

    __whexd_print_address(addr + bytes_read, mode.addr_fmt);
    __whexd_newline();

    return EXIT_SUCCESS;
}

void __whexd_print_help(void)
{
    printf("Help text goes here\n");
    exit(EXIT_SUCCESS);
}

void __whexd_print_version(void)
{
    printf("whexd (Windows Hex Dump) %s\n", VERSION);
    printf("Author: Zisis Avouris (Kavoureese on GitHub)");
    exit(EXIT_SUCCESS);
}

#endif /* WHEXD_H */
