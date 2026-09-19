#include <iostream>
#include <string>
#include "CliManager.h"
#include "Date.h"
#include "Food.h"
#include "Manager.h"

using namespace std;

int main(int argc, char* argv[])
{
    CliManager cli(argc, argv);
    cli.run();

    return 0;
}