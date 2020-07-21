#include <iostream>
#include "graphics/winDraw.h"
#include "graphics/winStudy.h"
#include "graphics/win3d.h"
#include "graphics/winPCurve.h"

using namespace std;

int main();

int readCommand(const string &cmd)
{
    static double zoom = 1;
    static int precision = 0;
    static bool drawGrid = true;
    static bool drawAxis = true;
    static bool rotating = true;
    static double curveFrom = 0;
    static double curveTo = 1;
    //static bool drawText = false;
    static map<string, string> functions;
    static map<string, double> symbols;
    if (cmd == "exit")
    {
        return 1;
    } else if (cmd == "help")
    {
        cout << "help:\n"
                "\twill show this message\n"
                "store|st [<name>=]<function>:\n"
                "\twill add function to the list of drawn functions\n"
                "\tunder the label <name>, if no \"=\"is found\n"
                "\tthe name will be the function itself\n"
                "remove|rm <name>|stored:\n"
                "\tif the arg is equal to \"stored\" all the stored functions\n"
                "\twill be removed from the list\n"
                "\twill remove the function under the label <name>\n"
                "list|ls:\n"
                "\twill display all the current labels and functions in the list\n"
                "\tin the format <name>=<function>\n"
                "storeVal|stv [<name>=]<expression>:\n"
                "\twill add a CONSTANT expression to the list of values\n"
                "\tunder the label <name>\n"
                "removeVal|rmv <name>|stored:\n"
                "\tif the arg is equal to \"stored\" all the stored values\n"
                "\twill be removed from the list\n"
                "\twill remove the value under the label <name>\n"
                "listVal|lsv:\n"
                "\twill display all the current labels and values in the list\n"
                "\tin the format <name>=<value>\n"
                "evaluate|eval <expression>\n"
                "\twill display the value of the expression (must be constant)\n"
                "\tafter all the values has been substituted\n"
                "draw <function>|<name>|stored:\n"
                "\tif the arg is equal to \"stored\" will draw all \n"
                "\tthe functions in the list, if the arg is a function it\n"
                "\twill draw the function defined in the args\n"
                "\tyou can use the name of a stored function to draw it\n"
                "\t(function must be R->R, with x as the independent variable)\n"
                "drawCurve <function>|<name>|(WIP:stored):\n"
                "\tif the arg is equal to \"stored\" will draw all \n"
                "\tthe functions in the list, if the arg is a function it\n"
                "\twill draw the function defined in the args\n"
                "\tyou can use the name of a stored function to draw it\n"
                "\t(function must be R->R^2, with t as the independent variable,\n"
                "\tthe format for a vector in R^2 is {x(t),y(t)})\n"
                "draw3d <function>|<name>\n"
                "\tsame as draw command but the domain is assumed to be R^2,\n"
                "\tso a 3d graph is shown (rotating by default, see set rotating)\n"
                "study <function>|<name>:\n"
                "\tdraws the study of the required function or stored function\n"
                "\thighlights maxima, minima and inflections, shows a different background\n"
                "\t\tred:\n"
                "\t\t\tincreasing interva\n"
                "\t\tblue:\n"
                "\t\t\tdecreasing interval\n"
                "\t\tyellow:\n"
                "\t\t\tconcave interval\n"
                "\t\tcyan:\n"
                "\t\t\tconvex interval\n"
                "\tif the function does not exist on a certain interval it will be\n"
                "\tcovered from a black red-crossed background\n"
                "derivative|diff [store] <function>|<name>:\n"
                "\tprints to the terminal the derivative of <function>\n"
                "\tif the name of a stored function is provided instead, it prints\n"
                "\tthe derivative of the stored function\n"
                "\tif the optional argument store is provided, it will store the derivative\n"
                "partial <variable> [store] <function>|<name>\n"
                "\tsame as derivative command but does a partial derivative on\n"
                "\tthe chosen variable\n"
                "set <parameter> <value>\n"
                "\tsets the parameter to the given value\n"
                "\tparameters:\n"
                "\t\tzoom (double):\n"
                "\t\t\tsets the zoom for the draw function\n"
                "\t\tprecision (int):\n"
                "\t\t\tsets how many lines are used for the draw function\n"
                "\t\tdrawGrid (bool):\n"
                "\t\t\tchoose if to draw the grid\n"
                "\t\tdrawAxis (bool):\n"
                "\t\t\tchoose if to draw the x and y axis\n"
                "\t\trotating (bool):\n"
                "\t\t\tchoose if the draw3d result is rotating\n"
                "\t\tcurveFrom (double):\n"
                "\t\t\tchose the 'a' value in the [a,b] interval in which the\n"
                "\t\t\tcurves are defined\n"
                "\t\tcurveTo (double):\n"
                "\t\t\tchose the 'b' value in the [a,b] interval in which the\n"
                "\t\t\tcurves are defined\n"
                //"\t\tdrawText (bool):(currently unavailable)\n"
                //"\t\t\tchoose if to draw the function in the left high corner\n"
                //"\tuse \"default\" as value to reset the value\n"
                "exit:\n"
                "\twill close the program\n"
                "\n"
                "credits to Ettore Ricci\n";
        return 0;
    } else if (cmd == "set")
    {
        string args;
        string tmp;
        cin >> args;
        if (args == "zoom")
        {
            cin >> tmp;
            if (tmp == "default")
                zoom = 1;
            else
                zoom = stod(tmp);
        } else if (args == "precision")
        {
            cin >> tmp;
            if (tmp == "default")
                precision = 0;
            else
                precision = stoi(tmp);
        } else if (args == "curveFrom")
        {
            cin >> tmp;
            if (tmp == "default")
                curveFrom = 0;
            else
                curveFrom = stod(tmp);
        } else if (args == "curveTo")
        {
            cin >> tmp;
            if (tmp == "default")
                curveTo = 1;
            else
                curveTo = stod(tmp);
        } else if (args == "drawGrid")
        {
            cin >> tmp;
            if (tmp == "true" || tmp == "default")
                drawGrid = true;
            else if (tmp == "false")
                drawGrid = false;
        } else if (args == "drawAxis")
        {
            cin >> tmp;
            if (tmp == "true" || tmp == "default")
                drawAxis = true;
            else if (tmp == "false")
                drawAxis = false;
        } else if (args == "rotating")
        {
            cin >> tmp;
            if (tmp == "true" || tmp == "default")
                rotating = true;
            else if (tmp == "false")
                rotating = false;
        }
        /*else if (args == "drawText")
        {
            cin >> tmp;
            if (tmp == "false" || tmp == "default")
                drawText = false;
            else if (tmp == "true")
                drawAxis = true;
        }*/
        return 0;
    } else if (cmd == "store" || cmd == "st")
    {
        string fun;
        cin >> fun;
        string fName = fun.substr(0, fun.find('='));
        fun.erase(0, fun.find('=') + 1);
        functions.insert({fName, fun});
        return 0;
    } else if (cmd == "remove" || cmd == "rm")
    {
        string name;
        cin >> name;
        if (name == "stored")
        {
            functions = {};
        }
        functions.erase(name);
        return 0;
    } else if (cmd == "list" || cmd == "ls")
    {
        for (auto const&[key, val] : functions)
        {
            if (key != val)
                cout << key << "=" << val << endl;
            else
                cout << val << endl;
        }
        return 0;
    } else if (cmd == "storeVal" || cmd == "stv")
    {
        string sym;
        cin >> sym;
        string sName = sym.substr(0, sym.find('='));
        sym.erase(0, sym.find('=') + 1);
        algebraParser sp(sym);
        symbols.insert({sName, sp.evaluate(symbols)});
        return 0;
    } else if (cmd == "removeVal" || cmd == "rmv")
    {
        string name;
        cin >> name;
        if (name == "stored")
        {
            symbols = {};
        }
        symbols.erase(name);
        return 0;
    } else if (cmd == "listVal" || cmd == "lsv")
    {
        for (auto const&[key, val] : symbols)
        {
            cout << key << "=" << val << endl;
        }
        return 0;
    } else if (cmd == "derivative" || cmd == "diff")
    {
        string args;
        string fun;
        bool doStore;
        cin >> args;
        if (args == "store")
        {
            cin >> fun;
            doStore = true;
        } else
        {
            fun = args;
            doStore = false;
        }
        if (functions.contains(fun))
        {
            auto f = algebraParser(functions[fun]);
            string der = f.derivative("x");
            if (doStore)
                functions.insert({fun + "'", der});
            else
                cout << fun << "'=" << der << endl;
        } else
        {
            auto f = algebraParser(fun);
            string der = f.derivative("x");
            if (doStore)
                functions.insert({"(" + fun + ")'", der});
            else
                cout << "(" << fun << ")'=" << der << endl;
        }
        return 0;
    } else if (cmd == "partial")
    {
        string var;
        string args;
        string fun;
        bool doStore;
        cin >> var >> args;
        if (args == "store")
        {
            cin >> fun;
            doStore = true;
        } else
        {
            fun = args;
            doStore = false;
        }
        if (functions.contains(fun))
        {
            auto f = algebraParser(functions[fun]);
            string der = f.derivative(var);
            if (doStore)
                functions.insert({fun + var, der});
            else
                cout << fun << var + "=" << der << endl;
        } else
        {
            auto f = algebraParser(fun);
            string der = f.derivative(var);
            if (doStore)
                functions.insert({"d" + var + "(" + fun + ")", der});
            else
                cout << "d" + var + "(" << fun << ")=" << der << endl;
        }
        return 0;
    } else if (cmd == "simplify")
    {
        string args;
        string fun;
        bool doStore;
        cin >> args;
        if (args == "store")
        {
            cin >> fun;
            doStore = true;
        } else
        {
            fun = args;
            doStore = false;
        }
        if (functions.contains(fun))
        {
            auto f = algebraParser(functions[fun]);
            string s = f.simplify();
            if (doStore)
                functions.insert({fun + "s", s});
            else
                cout << fun << "s=" << s << endl;
        } else
        {
            auto f = algebraParser(fun);
            string s = f.simplify();
            if (doStore)
                functions.insert({"(" + fun + ")'", s});
            else
                cout << "(" << fun << ")'=" << s << endl;
        }
        return 0;
    } else if (cmd == "evaluate" || cmd == "eval")
    {
        string args;
        cin >> args;
        if (functions.contains(args))
            args = functions[args];

        algebraParser toEv(args);
        cout << "=" << toEv.evaluate(symbols) << endl;


        return 0;
    } else if (cmd == "study")
    {
        string args;
        cin >> args;
        if (functions.contains(args))
            args = functions[args];

        drawStudy(args, -1, 0, 0, 0, precision, zoom, drawAxis, drawGrid);


        return 0;
    } else if (cmd == "draw")
    {
        string args;
        cin >> args;
        map<string, string> toDraw;
        if (args == "stored")
        {
            if (functions.empty())
                return -1;
            else
                toDraw = functions;
        } else if (functions.contains(args))
        {
            toDraw[args] = functions[args];
        } else
        {
            toDraw = {{"f", args}};
        }

        drawGraph(toDraw, 0, 0, 0, 0, precision, zoom, drawAxis, drawGrid);

        return 0;
    } else if (cmd == "drawCurve")
    {
        string args;
        cin >> args;
        string toDraw;
        /*if (args == "stored")
        {
            if (functions.empty())
                return -1;
            else
                toDraw = functions;
        }*/
        if (functions.contains(args))
        {
            toDraw = functions[args];
        } else
        {
            toDraw = args;
        }

        drawPCurve(toDraw, 0, 0, 0, 0, precision, zoom, drawAxis, drawGrid, curveFrom, curveTo);

        return 0;
    } else if (cmd == "draw3d")
    {
        string args;
        cin >> args;
        string toDraw;
        if (functions.contains(args))
        {
            toDraw = functions[args];
        } else
        {
            toDraw = args;
        }
        draw3d(toDraw, precision, zoom, drawAxis, drawGrid, rotating);
        return 0;
    } else if (cmd == "field3d")
    {
        string args;
        cin >> args;
        string toDraw;
        if (functions.contains(args))
        {
            toDraw = functions[args];
        } else
        {
            toDraw = args;
        }
        ///wip
        ///field3d(toDraw, precision, zoom, drawAxis, drawGrid, rotating);
        return 0;
    }
    return -1;
}

int main()
{
    string operation;
    cout << "Welcome to BRANCHES\n"
            "a simple graphing terminal\n"
            "by Ettore Ricci\n\n"
            "type \"help\" for a list of all the available commands\n\n";
    int ret = 0;
    while (ret != 1)
    {
        cout << (ret == 0 ? ">>" : "x>");
        cin >> operation;
        ret = readCommand(operation);
    }

    return 0;
}
