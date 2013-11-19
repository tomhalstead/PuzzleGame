#include <iostream>
#include <fstream>
#include <QString>
#include <QStringList>

using namespace std;

int main()
{
    QString input;
    QStringList list;
    int numTiles;
    std::string fileNameIn, fileNameOut;
    cout << "File: ";
    getline(cin,fileNameIn);
    cout << "NumTiles: ";
    cin >> numTiles;
    ifstream in;
    ofstream out;
    in.open(fileNameIn.c_str());
    std::string c;
    while(getline(in,c)) {
        input.append(c.c_str());
    }
    in.close();
    input.replace(" ","");
    list = input.split(",");
    fileNameOut = fileNameIn + ".tileset.txt";
    out.open(fileNameOut.c_str());
    out << "\t\t<tileset id=\"\" fileName=\"\">" << endl;
    for(int i = 0; i < numTiles; i++)
        out << "\t\t\t<tile id=\"" << i << "\" draw=\"" << i << "\"/>" << endl;
    out << "\t\t</tileset>" << endl;
    out.close();
    fileNameOut = fileNameIn + ".tiles.txt";
    out.open(fileNameOut.c_str());
    for(int i = 0; i < list.count(); i++)
        out << "\t\t\t\t<location type=\"" << list.at(i).toInt()-1 << "\"/>" << endl;
    out.close();
    return 0;
}

