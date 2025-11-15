#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "scene.h"
#include "display.h"
#include "display.h"
#include "surface.h"

using namespace std;

int main(int argc, char *argv[])
{
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
        getline(std::cin, cmd);

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
                scene.cmd_display(std::make_unique<DisplayTextures>());
            else if (cmd == "o2")
                scene.cmd_display(std::make_unique<DisplayIDs>());
            else
                std::cout << "Commande inconnue\n";
            break;
        }

        case 'f':
        {
            std::cout << "Entrez les IDs des points à fusionner séparés par des espaces:\n> ";
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int id;
            std::vector<int> ids;
            while (iss >> id)
                ids.push_back(id);
            scene.cmd_merge_createCloud(ids);
            break;
        }

        case 'd':
        {
            std::cout << "Entrez ID puis nouvelle position x y:\n> ";
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int id, x, y;
            if (iss >> id >> x >> y)
            {
                if (!scene.cmd_movePoint(id, x, y))
                    std::cout << "ID invalide\n";
            }
            else
                std::cout << "Format invalide\n";
            break;
        }

        case 's':
        {
            std::cout << "Entrez ID du point à supprimer:\n> ";
            std::string line;
            std::getline(std::cin, line);
            int id = std::stoi(line);
            if (!scene.cmd_deletePoint(id))
                std::cout << "ID invalide\n";
            break;
        }

        case 'c':
        {
            if (cmd == "c1")
                scene.cmd_buildSurface(std::make_unique<SurfaceByIDOrder>());
            else if (cmd == "c2")
                scene.cmd_buildSurface(std::make_unique<SurfaceByNearest>());
            else
                std::cout << "Commande inconnue\n";
            break;
        }

        default:
            std::cout << "Commande inconnue\n";
        }
    }

    return 0;
}