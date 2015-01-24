#include <console.h>
#include "experiment.h"
#include <physmem.h>
#include <io.h>

void experiment() {

    printk("TESTING SERIAL PORT\n");

    init_serial();
    write_serial('w');
    write_serial('i');
    write_serial('k');
    write_serial('t');
    write_serial('o');
    write_serial('r');
    write_serial('!');
}
