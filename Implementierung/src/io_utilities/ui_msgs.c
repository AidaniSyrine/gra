
#include "ui_msgs.h"

void print_welcome(void) {
    printf("==== Tonwertkorrektur, a PPM(P6) tonal corrector.\n"); 
}

void print_goodbye(int state) {
    if (state == 1) 
        printf("==== Summary: 1 image has been converted. 0 Errors have been detected.\n");
    else if (!state)
        printf("==== Summary: 0 image is converted. At least 1 Error have been detected\n");
    else 
        printf("==== Summary: 0 image is converted. 0 Error have been detected\n");
}

void print_help(void){
    printf("\n"
        "tonwertkorrektur - Apply tonal correction to PPM P6 images\n\n"

        "DESCRIPTION\n"
        "       Tonwertkorrektur is a command-line utility that reads a PPM P6 image file, converts it to grayscale,\n"
        "       applies tonal correction according to user-specified options, and writes the transformed image to a\n" 
        "       PGM P5 file.\n"
        "\n"
        "USAGE\n"
        "       tonwertkorrektur <input_image>\n"
        "                   The input file is a positional argument.\n"
        "\n"
        "OPTIONS\n"
        "       All of the single-character tonwertkorrektur options documented in the description can be\n" 
        "       interpreted in both upper case and lower case.\n"
        "       All arguments, whether single-character or multi-character arguments are mandatory unless\n" 
        "       explicitly stated otherwise.\n"
        "       -v[int]     Specifies the implementation to be used. -v0 for main implementation. -v1 and -v2 are\n"
        "                   for two comparison implementations. If not specified, main implementation will be used\n"
        "                   by default.\n"                                           
        "       -b[int]     Measures and outputs the runtime of the specified implementation. The optional argument\n"
        "                   specifies the number of repetitions of the function call. If the  optional argument not\n"
        "                   specified,the function call is repeated three times by default.\n"
        "       -o[output_img]\n"                         
        "                   Specifies the output path image to be written to. If not specified image will be saved\n"
        "                   in Implemetierung/resources/result.\n"
        "\n"
        "       The following options must appear on the command line either without an input path or, at the very\n"
        "       least, before an input file to ensure correct processing\n"
        "\n"
        "       -h, --help\n"
        "                   Displays help window.\n"
        "       -t, --test\n"
        "                   Displays testing results and synthesis.\n"  
        "\n"             
        "       tonwertkorrektur also interprets a number of other multi-character options.\n"
        "\n"
        "       --lvlss[double],[double]\n"
        "                   Sets the contrast levels. If not specified, the default levels of (0,0) will be applied.\n"
        "       --lvlsm[double],[double]\n"
        "                   Sets the midtone levels. If not specified an average of brightness and contrast levels\n"
        "                   will be applied.\n"
        "       --lvlsw[double],[double]\n"
        "                   Sets the brightness levels. If not specified the default levels of (255, 255) will be \n"
        "                   applied.\n"
        "       --coeffs[double],[double],[double]\n"
        "                   Sets the coefficients for grayscale conversion. If not specified, internally defined \n"
        "                   coefficients will be applied.\n"
        "\n"
        "AUTHOR\n"
        "                   Adem Trabelsi, Ahmed Mhamdi, Syrine Aidani\n"
        "\n"
        "REPORTING BUGS\n"
        "                   Hopefully not :)\n"
        "\n"
        "SEE ALSO\n"
        "                   GRA_0219.pdf(1),    Ausarbeitung/Ausarbeitung.pdf(2)\n"
        "\n"
        );
}
