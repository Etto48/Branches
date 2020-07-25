//
// Created by the_E on 20/07/25.
//

#ifndef BRANCHES_COLORS_H
#define BRANCHES_COLORS_H

#include <string>
#include <map>

namespace color
{
    bool isColored;
}
std::string colorize(const std::string &text, const std::string &fg = "Default", const std::string &bg = "Default",
                     const std::string &format = "Default", const std::string &reset = "All")
{
    if (color::isColored)
    {
        std::map<std::string, std::string> formatMap(
                {{"Default",    "0"},
                 {"Bold",       "1"},
                 {"Dim",        "2"},
                 {"Underlined", "3"},
                 {"Blink",      "5"},
                 {"Reverse",    "7"},
                 {"Hidden",     "8"}}
        );
        std::map<std::string, std::string> foregroundMap(
                {{"Default",       "39"},
                 {"Black",         "30"},
                 {"Red",           "31"},
                 {"Green",         "32"},
                 {"Yellow",        "33"},
                 {"Blue",          "34"},
                 {"Magenta",       "35"},
                 {"Cyan",          "36"},
                 {"Light Gray",    "37"},
                 {"Dark Gray",     "90"},
                 {"Light Red",     "91"},
                 {"Light Green",   "92"},
                 {"Light Yellow",  "93"},
                 {"Light Blue",    "94"},
                 {"Light Magenta", "95"},
                 {"Light Cyan",    "96"},
                 {"White",         "97"}}
        );
        std::map<std::string, std::string> backgroundMap(
                {{"Default",       "49"},
                 {"Black",         "40"},
                 {"Red",           "41"},
                 {"Green",         "42"},
                 {"Yellow",        "43"},
                 {"Blue",          "44"},
                 {"Megenta",       "45"},
                 {"Cyan",          "46"},
                 {"Light Gray",    "47"},
                 {"Dark Gray",     "100"},
                 {"Light Red",     "101"},
                 {"Light Green",   "102"},
                 {"Light Yellow",  "103"},
                 {"Light Blue",    "104"},
                 {"Light Magenta", "105"},
                 {"Light Cyan",    "106"},
                 {"White",         "107"}}
        );
        std::map<std::string, std::string> resetMap(
                {{"All",        "0"},
                 {"Bold",       "21"},
                 {"Dim",        "22"},
                 {"Underlined", "24"},
                 {"Blink",      "25"},
                 {"Reverse",    "27"},
                 {"Hidden",     "28"}}
        );
        ///"\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"
        return "\033[" + formatMap[format] + ";" + foregroundMap[fg] + ";" + backgroundMap[bg] + "m" + text +
               (reset != "None" ? ("\033[" + resetMap[reset] + "m") : "");
    } else
    {
        return text;
    }
}

#endif //BRANCHES_COLORS_H
