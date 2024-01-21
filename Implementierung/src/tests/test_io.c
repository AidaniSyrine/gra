#include "../io_utilities/img_io.h"
#include "test_io.h"

Test(image_input_output, one_fn) {
    int val = one(); 
    cr_expect(val == 1, "One should be 1\n");
}
