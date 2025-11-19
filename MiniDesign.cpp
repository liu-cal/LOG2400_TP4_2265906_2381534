#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "scene.h"
#include "display.h"
#include "display.h"
#include "surface.h"
#include <windows.h>

using namespace std;

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    string args;
    // On accepte des points en entrée.
    if (argc > 1)
    {
        ostringstream oss;
        for (int i = 1; i < argc; ++i)
            oss << argv[i] << " ";
        args = oss.str();
    }
    else
    {
        cout << "Entrez les points au format (x,y) :\n> ";
        getline(cin, args);
    }

    // Voici des fonctions utiles pour réaliser le TP.
    // TODO: Il faudrait les placer dans des classes appropriées.
    Scene scene;
    scene.initFromArgsString(args);

    // vector<Point> points = creerPoints(args);
    // imprimerGrille(points);

    // Ce sont différentes textures possibles. Seules les 2 premières sont utilisées dans les scénarios du TP.
    // vector<char> texturesNuages = {'o', '#', '$'};
    string cmd;

    // Menu
    while (true)
    {
        cout << "\nCommandes:\n"
             << "a  - Afficher les points et les nuages\n"
             << "o1 - Afficher l'orthèse avec les textures des points\n"
             << "o2 - Afficher l'orthèse avec les IDs des points\n"
             << "f  - Fusionner des points dans un nuage (et appliquer texture)\n"
             << "d  - Deplacer un point (ID)\n"
             << "s  - Supprimer un point (ID)\n"
             << "c1 - Créer les surfaces selon l'ordre des IDs\n"
             << "c2 - Créer les surfaces selon la distance minimale\n"
             << "q  - Quitter\n> ";
        getline(cin, cmd);

        switch (cmd[0])
        {

        case 'q':
            return 0;

        case 'a':
            scene.cmd_list();
            break;

        case 'o':
        {
            if (cmd == "o1")
                scene.cmd_display(make_unique<DisplayTextures>());
            else if (cmd == "o2")
                scene.cmd_display(make_unique<DisplayIDs>());
            else
                cout << "Commande inconnue\n";
            break;
        }

        case 'f':
        {
            cout << "IDs des points à fusionner dans un nuage (ex: 0 2 4): ";
            string line;
            getline(cin, line);
            istringstream iss(line);
            int id;
            vector<int> ids;
            while (iss >> id)
                ids.push_back(id);
            scene.cmd_merge_createCloud(ids);
            break;
        }

        case 'd':
        {
            cout << "ID du point à déplacer: ";
            string line;
            getline(cin, line);
            int id = stoi(line);

            cout << "Nouvelle position (x y): ";
            int x, y;
            getline(cin, line);
            istringstream iss(line);

            if (iss >> x >> y)
            {
                if (!scene.cmd_movePoint(id, x, y))
                    cout << "ID invalide\n";
            }
            else
            {
                cout << "Format invalide\n";
            }
            break;
        }

        case 's':
        {
            cout << "ID du point à supprimer: ";
            string line;
            getline(cin, line);
            int id = stoi(line);
            if (!scene.cmd_deletePoint(id))
                cout << "ID invalide\n";
            break;
        }

        case 'c':
        {
            if (cmd == "c1")
                scene.cmd_buildSurface(make_unique<SurfaceByIDOrder>());
            else if (cmd == "c2")
                scene.cmd_buildSurface(make_unique<SurfaceByNearest>());
            else
                cout << "Commande inconnue\n";
            break;
        }

        default:
            cout << "Commande inconnue\n";
        }
    }

    return 0;
}