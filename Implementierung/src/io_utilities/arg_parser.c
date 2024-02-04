//
// Created by Adem Trabelsi on 12.01.24.
//
#include "arg_parser.h"


int test_and_set_sarg(int* valid_arg, const char* option_arg) {
    errno = 0;
    char* endptr;

    *valid_arg = (int) strtol(option_arg, &endptr, 10);
    if (endptr == option_arg || *endptr != '\0' || errno == ERANGE) {
        fprintf(stderr, "Failed! Invalid short argument option.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int test_and_set_largs(float* valid_args, char* option_args, int flag) {
    char* token = NULL;
    char* endptr;
    size_t i;
    for(token = strtok(option_args, ","), i = 0;
        token != NULL; 
        token = strtok(NULL, ","), i++) {
            errno = 0;
            
            float tmp = strtof(token, &endptr);
            if(endptr == token || *endptr != '\0' ||errno == ERANGE) {
                fprintf(stderr, "Failed! Invalid option(s).\n");
                return EXIT_FAILURE;
            }
            
            if (flag == 'c') {
                if (i > 2 || tmp < 0 || (tmp != tmp) ||  isinf(tmp)) {
                    fprintf(stderr, "Failed! Invalid coefficient option(s).\n");
                    return EXIT_FAILURE;
                }
                valid_args[i] = tmp;
            } else {
                if (i > 1 || tmp > 255 || tmp < 0 || tmp != (int)tmp || (tmp != tmp) || isinf(tmp)) {
                    fprintf(stderr, "Failed! Invalid long option(s).\n");
                    return EXIT_FAILURE;
                }
                valid_args[i] = tmp;
            }
        }
        if (flag == 'c') {
                if (i != 3 ) {
                    fprintf(stderr, "Failed! Missing coefficient option(s).\n");
                    return EXIT_FAILURE;
                }
            } else {
                if (i != 2 ) {
                    fprintf(stderr, "Failed! Missing long option(s).\n");
                    return EXIT_FAILURE;
                }

            }
        return EXIT_SUCCESS;
}

int test_and_set_input(Input_params* input_params, const char* arg) {
    // Input file should exist and be accessible
    if (access(arg, F_OK)) {
        fprintf(stderr, "Failed! accessing input file: %s.\n", arg);
        return EXIT_FAILURE;
    }
    // Allocate mem 
    size_t input_path_len = strlen(arg) + 1; 
    input_params->input_img_path = (char *) malloc(input_path_len * sizeof (char)); 
    if (input_params->input_img_path == NULL) {
        fprintf(stderr, "Failed! allocating memory for input file.\n"); 
        return EXIT_FAILURE; 
    }
    strncpy(input_params->input_img_path, arg, input_path_len);
    return EXIT_SUCCESS; 
}


int test_and_set_output(Input_params* input_params, const char* arg) {
    size_t suffix_len = strlen(".pgm");
    size_t arg_len = strlen(arg);

    // .pgm as an output file name is not allowed
    if (arg_len < suffix_len + 1) {
        fprintf(stderr, "Failed! Empty file name\n"); 
        return EXIT_FAILURE;
    }
    // Non-pgm files are also not allowed
    if (strncmp(arg + arg_len - suffix_len, ".pgm", suffix_len)) {
        fprintf(stderr, "Failed! Non PGM file\n");
        return EXIT_FAILURE;
    }
    // Allocate mem 
    size_t output_path_len = strlen(arg) + 1; 
    input_params->output_img_path = (char *) malloc(output_path_len * sizeof (char));
    if (input_params->output_img_path == NULL) { 
        fprintf(stderr, "Failed allocating memory for output file.\n");
        return EXIT_FAILURE;
    }
    strncpy(input_params->output_img_path, arg, output_path_len);
    return EXIT_SUCCESS;
}



int arg_parser(Input_params* input_params, int argc, char* argv[]) { 
    // get-opt variables
    int option_index = 0;
    int option;
    opterr = 0;
    char* optstring = "-:HTO:V:B::hto:v:b::";  
    
    // Flags
    int input_flag = 0, output_flag = 0, vflag = 0, bflag = 0;
    // Tmp for longopts 
    float valid_args[3];
   
    while((option = getopt_long(argc, argv, optstring , longopts, &option_index)) != EOF) {
            switch(option) {
                case 0: // Handle longopts
                    switch (longopts[option_index].val) {
                        case 'c':
                            if (test_and_set_largs(valid_args, optarg, cflag)) return EXIT_FAILURE;
                            input_params->a = valid_args[0]; 
                            input_params->b = valid_args[1];
                            input_params->c = valid_args[2];
                            break;
                        case 's':
                            if (test_and_set_largs(valid_args, optarg, sflag)) return EXIT_FAILURE;
                            input_params->es = valid_args[0];
                            input_params->as = valid_args[1];
                            break;
                        case 'm':
                            if (test_and_set_largs(valid_args, optarg, mflag)) return EXIT_FAILURE;
                            input_params->em = valid_args[0]; 
                            input_params->am = valid_args[1];
                            break;
                        case 'w':
                            if (test_and_set_largs(valid_args, optarg, wflag)) return EXIT_FAILURE;
                            input_params->ew = valid_args[0];
                            input_params->aw = valid_args[1];
                            break;
                    } break;
                case 1: // Non-option arg input file
                    if (input_flag) goto arg_error;
                    if (test_and_set_input(input_params, optarg))
                        return EXIT_FAILURE;
                    input_flag++;
                    printf("Given input path: %s\n", input_params->input_img_path);
                    break;
                case 'o': case 'O':
                    if (output_flag) goto arg_error;
                    if (test_and_set_output(input_params, optarg))
                        return EXIT_FAILURE; 
                    output_flag++;
                    printf("Given output path: %s\n", input_params->output_img_path);
                    break;
                case 'V': case 'v':
                    if (vflag) goto arg_error;
                    if (test_and_set_sarg(&(input_params->version), optarg)) 
                        return  EXIT_FAILURE;
                    if (input_params->version < 0 || input_params->version > 3) {
                        fprintf(stderr, "Failed! Version number is undefined. Try --help\n");
                        return EXIT_FAILURE; 
                    }
                    vflag++;
                    break;
                case 'B': case 'b':
                    if (bflag) goto arg_error;
                    bflag++;
                    if (!optarg) {
                        input_params->iter_num = DEFAULT_ITER_NUM; 
                        printf("Default iteration depth: %d iterations.\n", input_params->iter_num);
                    } else {
                        if (test_and_set_sarg(&(input_params->iter_num), optarg))
                            return EXIT_FAILURE;
                        if (input_params->iter_num <= 0) {
                            fprintf(stderr, "Failed! Zero or non positive iteration depth.\n");
                            return EXIT_FAILURE; 
                        }
                        printf("Given iteration depth: %d iterations.\n", input_params->iter_num);
                    }
                    break;
                case 't': case 'T': 
                    input_params->enable_tests = true;
                    return EXIT_SUCCESS; 
                case 'h': case 'H':
                    input_params->enable_help = true; 
                    return EXIT_SUCCESS;
                case ':':
                    switch (optopt) {
                        case 'v': case 'V':
                            fprintf(stderr, "Failed! Version is NOT provided. See -V <version_num>\n");
                            return EXIT_FAILURE;
                        case 'o': case 'O':
                            fprintf(stderr, "Failed! Output file is NOT provided. See -o <output_path>\n");
                            return EXIT_FAILURE; 
                    } break; 
                case '?':
                    fprintf(stderr, "Failed! Unknown option: -%c. Try --help.\n", optopt);
                    return EXIT_FAILURE; 
            }
    }

    // No input file
    if (!input_flag) { 
        fprintf(stderr, "Failed! Positional argument is NOT provided. Try --help.\n");
        return EXIT_FAILURE; 
    }
    
    // Adjust middle brightness and contrast values
    if (!mflag) {
        input_params->em = (int) (input_params->es + input_params->ew) / 2;
        input_params->am = (int) (input_params->as + input_params->aw) / 2;
    }
    
    // Check contrast and brightness values
    if (sflag || mflag || wflag)
        if (input_params->es >= input_params->ew || input_params->es >= input_params->em
            || input_params->ew <= input_params->em || input_params->as >= input_params->aw
            || input_params->as >= input_params->am || input_params->aw <= input_params->am) {
            fprintf(stderr, "Failed! Invalid long option(s).\n"
            "Check provided contrast and brightness values\n");
            return EXIT_FAILURE;
        }
        
    // Default output file
    if (!output_flag) {
        test_and_set_output(input_params, DEFAULT_OUTPUT_PATH); 
        printf("Default output path: %s\n", input_params->output_img_path);
    }
    
    return EXIT_SUCCESS;

    arg_error: 
        fprintf(stderr, "Failed! One or more arguments provided do not match their definitions . Try --help.\n"); 
        return EXIT_FAILURE;
}

void dealloc_input_params(Input_params* input_params) {
    if(input_params->input_img_path != NULL) {
        free(input_params->input_img_path); 
    }
    if(input_params->output_img_path != NULL) {
        free(input_params->output_img_path);
    }
}
