#include "whexd.h"

int main(int argc, char *argv[])
{
    whexd_mode_t mode;
    whexd_init_mode(&mode);
    
    char filename[_MAX_PATH];
    if (!__whexd_parse_args(argc, argv, &mode, &filename[0]))
        return EXIT_FAILURE;

    return whexdump(&filename[0], &mode);
}
