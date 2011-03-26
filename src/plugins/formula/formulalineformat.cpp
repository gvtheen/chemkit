/******************************************************************************
**
** Copyright (C) 2009-2011 Kyle Lutz <kyle.r.lutz@gmail.com>
**
** This file is part of chemkit. For more information see
** <http://www.chemkit.org>.
**
** chemkit is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** chemkit is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with chemkit. If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#include "formulalineformat.h"

FormulaLineFormat::FormulaLineFormat()
    : chemkit::LineFormat("formula")
{
}

bool FormulaLineFormat::read(const std::string &formula, chemkit::Molecule *molecule)
{
    bool inSymbol = false;
    bool inNumber = false;
    QString symbol;
    QString number;

    foreach(const char &c, formula){
        if(isspace(c)){
            continue;
        }
        else if(isdigit(c)){
            if(inSymbol){
                inSymbol = false;
            }

            if(inNumber){
                number += c;
            }
            else{
                inNumber = true;
                number = c;
            }
        }
        else if(isalpha(c)){
            if(inNumber){
                for(int i = 0; i < number.toInt(); i++){
                    molecule->addAtom(symbol.toStdString());
                }

                number.clear();
                inNumber = false;
            }

            if(inSymbol && islower(c)){
                symbol += c;
            }
            else if(inSymbol && isupper(c)){
                molecule->addAtom(symbol.toStdString());
                symbol = c;
            }
            else{
                symbol = c;
                inSymbol = true;
            }
        }
    }

    if(!symbol.isEmpty()){
        if(number.isEmpty()){
            number = "1";
        }

        for(int i = 0; i < number.toInt(); i++){
            molecule->addAtom(symbol.toStdString());
        }
    }

    return true;
}

std::string FormulaLineFormat::write(const chemkit::Molecule *molecule)
{
    return molecule->formula();
}

