namespace netgen
{
const char * hexrules[] = {
"rule \"Hexa left-right-top\"\n",\
"\n",\
"quality 1\n",\
"\n",\
"flags t;\n",\
"\n",\
"mappoints\n",\
"(0, 0, 0);\n",\
"(1, 0, 0) { 1 } ;\n",\
"(1, 1, 0) { 1 } ;\n",\
"(0, 1, 0) { 1 } ;\n",\
"(0, 0, 1) { 1 } ;\n",\
"(1, 0, 1) { 1 } ;\n",\
"(1, 1, 1) { 1 } ;\n",\
"(0, 1, 1) { 1 } ;\n",\
"\n",\
"mapfaces\n",\
"(4, 3, 2, 1) del;\n",\
"(3, 7, 6, 2) del;\n",\
"(7, 8, 5, 6) del;\n",\
"(8, 4, 1, 5) del;\n",\
"\n",\
"newpoints\n",\
"\n",\
"newfaces\n",\
"(5, 6, 2, 1);\n",\
"(7, 8, 4, 3);\n",\
"\n",\
"elements\n",\
"(4, 3, 2, 1, 8, 7, 6, 5);\n",\
"\n",\
"freezone2\n",\
"{ 1 P1 };\n",\
"{ 1 P2 };\n",\
"{ 1 P5 };\n",\
"{ 1 P3 };\n",\
"\n",\
"\n",\
"endrule\n",\
"\n",\
"\n",\
"\n",\
"\n",\
"\n",\
"rule \"Hexa fill\"\n",\
"\n",\
"quality 10\n",\
"\n",\
"flags t;\n",\
"\n",\
"mappoints\n",\
"(0, 0, 0);\n",\
"(1, 0, 0) { 1 } ;\n",\
"(1, 1, 0) { 1 } ;\n",\
"(0, 1, 0) { 1 } ;\n",\
"(0, 0, 1) { 1 } ;\n",\
"(1, 0, 1) { 1 } ;\n",\
"(1, 1, 1) { 1 } ;\n",\
"(0, 1, 1) { 1 } ;\n",\
"\n",\
"mapfaces\n",\
"(4, 3, 2, 1) del;\n",\
"(3, 7, 6, 2) del;\n",\
"(7, 8, 5, 6) del;\n",\
"(8, 4, 1, 5) del;\n",\
"(1, 2, 6, 5) del;\n",\
"(3, 4, 8, 7) del;\n",\
"\n",\
"newpoints\n",\
"\n",\
"newfaces\n",\
"\n",\
"elements\n",\
"(4, 3, 2, 1, 8, 7, 6, 5);\n",\
"\n",\
"freezone2\n",\
"{ 1 P1 };\n",\
"{ 1 P2 };\n",\
"{ 1 P5 };\n",\
"{ 1 P3 };\n",\
"\n",\
"\n",\
"endrule\n",\
"\n",\
"\n",\
"\n",\
"\n",\
"\n",\
"\n",\
0};
}
