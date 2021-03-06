/*
 * AIM script2txt
 * Copyright (C) 2015 lzwdgc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iostream>
#include <stdint.h>

#include "ParserDriver.h"
#include "script.h"

using std::cout;
using std::string;

int main(int argc, char *argv[])
try
{
    if (argc != 2)
    {
        cout << "Usage:\n" << argv[0] << " script.scr";
        return 1;
    }

    std::string filename = argv[1];

    // read
    buffer b(readFile(filename));
    script s;
    s.load(b);
    auto str = s.get_text();

    ParserDriver driver;
    if (driver.parse(str))
    {
        throw std::runtime_error("error during parsing input file");
    }
    auto &ctx = driver.getContext();

    // write script
    {
        filename += ".txt";
        std::ofstream ofile(filename);
        if (ofile)
            ofile << ctx.getText();
    }

    // write function calls
    {
        std::ofstream functions("functions.txt", std::ios::app);
        if (functions)
        {
            for (auto &f : driver.functions)
            {
                std::string f2(f.size(), 0);
                std::transform(f.begin(), f.end(), f2.begin(), tolower);
                functions << f2 << "\n";
            }
        }
    }

    return 0;
}
catch (std::exception &e)
{
    printf("error: %s\n", e.what());
    return 1;
}
catch (...)
{
    printf("error: unknown exception\n");
    return 1;
}
