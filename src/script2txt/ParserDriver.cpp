/*
 * Polygon-4 script2txt
 * Copyright (C) 2015-2016 lzwdgc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ParserDriver.h"

#include <algorithm>
#include <fstream>

// Prevent using <unistd.h> because of bug in flex.
#define YY_NO_UNISTD_H 1
#define YY_DECL 1
#include <lexer.h>
extern yy::parser::symbol_type yylex(yyscan_t yyscanner, yy::location &loc);

ParserDriver::ParserDriver()
{
    debug = false;
}

yy::parser::symbol_type ParserDriver::lex()
{
    return yylex(scanner, location);
}

int ParserDriver::parse(const std::string &s)
{
    yylex_init(&scanner);
    yy_scan_string(s.c_str(), scanner);

    yy::parser parser(*this);
    parser.set_debug_level(debug);
    int res = parser.parse();

    yylex_destroy(scanner);

    return res;
}

void ParserDriver::error(const yy::location &l, const std::string &m)
{
    std::cerr << l << " " << m << std::endl;
}

void ParserDriver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}
