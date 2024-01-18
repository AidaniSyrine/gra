
#include "ui_msgs.h"


// TODO
void print_help(void){
    puts("\n"
         "Desciption:\n"
         "--to edit--\n"
         "Usage:\n"
         "      tonwertkorrktur <input_image>\n"
         "NOTE: *.ppm                           Input image MUST have a 24bpp PPM Format (P6)\n"
         "\n"
         "Optional Arguments:\n"
         "  -V<int>                             ---\n"
         "  -B<int>                             ---\n"
         "  -o<output_img>                      ---\n"
         "NOTE:\n"
         " *.ppm                                Output image MUST have a 24bpp PPM Format (P6).\n"
         "  --coeffs<double>,<double>,<double>  ---\n"
         "  --lvlss<double>,<double>            ---\n"
         "  --lvlsm<double>,<double>            ---\n"
         "  --lvlsw<double>,<double>            ---\n"
         "  -h|--help                           ---\n"
         );
}
//TODO
void print_welcome(void) {
    printf("WELCOME!!!\n");
}

//TODO
void print_runtime(int iter_num, double time) {
    printf("%d-times formatting took %f seconds\n", iter_num, time);
}

//TODO
void print_goodbye(void) {
    printf("END!!!\n");
}