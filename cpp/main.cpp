#include <iostream>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "tcpserver.h"
#include "multimedia.h"
#include "photo.hpp"
#include "video.hpp"
#include "film.hpp"
#include "group.hpp"
#include "database.h"
#include <vector>

using namespace std;

#define VERSION_ETAPE_11
#ifdef VERSION_ETAPE_11

const int PORT = 3331;


int main(int argc, const char* argv[])
{
    Database db;

    db.createPhoto("pomme", "pomme.jpg");
    db.createVideo("cheval", "cheval.mp4", 26);
    int durations[] = {10, 15};
    db.createFilm("cheval2", "cheval2.mp4", 120, durations, 2);

    auto* server =
    new TCPServer( [&](std::string const& request, std::string& response) {

        // the request sent by the client to the server
        std::cout << "request: " << request << std::endl;
        std::string answer{};

        if (request.find("find media") != std::string::npos)                 // faore une méthode dans database 
        {
            std::string search = "find media ";                                       // découper la requête avant
            size_t pos = request.find(search);  // Trouver la position de "create photo"
            std::string result = request.substr(pos + search.length());

            std::stringstream out;
            
            db.findMedia(result, out);
            answer = out.str();
        }
        else if (request.find("find group") != std::string::npos) 
        {
            std::string search = "find group ";  
            size_t pos = request.find(search);  // Trouver la position de "create photo"
            std::string result = request.substr(pos + search.length());

            std::stringstream out;

            db.findGroup(result, out);
            answer = out.str();
        }
        else if (request.find("play") != std::string::npos) 
        {
            std::string search = "play ";  
            size_t pos = request.find(search);  // Trouver la position de "create photo"
            std::string result = request.substr(pos + search.length());

            db.playMedia(result);
            answer = "Playing " + result;
        } 
        else 
        {
            response = "Commande non reconnue";
        }

        // the response that the server sends back to the client
        response = "RECEIVED: " + answer;

        // return false would close the connection with the client
        return true;
    });


    // lance la boucle infinie du serveur
    std::cout << "Starting Server on port " << PORT << std::endl;

    int status = server->run(PORT);

    // en cas d'erreur
    if (status < 0) {
        std::cerr << "Could not start Server on port " << PORT << std::endl;
    return 1;
    }

    return 0;
}



#else 


void test_multimedia() {
    Multimedia *m = new Video("nom", "chemin");
    std::cout << m->getName() << std::endl;
    m->printValues(std::cout);
    
    delete m;
}

void play_photo() {
    Photo *p = new Photo("pomme", "pomme.jpg", 0, 0);
    p->play();
    delete p;
}

void play_video() {
    Video *v = new Video("cheval", "cheval.mp4");
    v->play();
    delete v;
}

void play_table() {
    std::vector<Multimedia*> multimediaList;
    multimediaList.push_back(new Photo("pomme", "pomme.jpg"));
    multimediaList.push_back(new Video("cheval", "cheval.mp4"));
    multimediaList.push_back(new Photo("pomme", "pomme.jpg"));
    for (const auto& multimedia : multimediaList) {
        std::cout << "=== Affichage des informations ===" << std::endl;
        multimedia->play();
        std::cout << std::endl;
    }

    for (const auto& multimedia : multimediaList) {
        delete multimedia;
    }
}

void test_chapters() {
    int durations[3] = {10, 6, 10};
    Film f("cheval", "cheval.mp4", 26, durations, 3);
    f.play();
    f.printValues(std::cout);
}

void test_film() {
    int durations[3] = {10, 6, 10};
    Film f("cheval", "cheval.mp4", 26, durations, 3);
    f.printValues(std::cout);
}

void groups()
{
    Group group1("groupe 1");
    Group group2("groupe 2");

    group1.push_back(std::make_shared<Photo>("pomme", "pomme.jpg"));
    group1.push_back(std::make_shared<Photo>("rio", "rio.jpg"));
    group1.push_back(std::make_shared<Photo>("telecom", "telecom.jpg"));

    group2.push_back(std::make_shared<Photo>("pomme", "pomme.jpg"));
    group2.push_back(std::make_shared<Video>("cheval", "cheval.mp4"));
    int durations[3] = {10, 6, 10};
    group2.push_back(std::make_shared<Film>("cheval", "cheval.mp4", 26, durations, 3));

    group1.printValues(std::cout);
    group2.printValues(std::cout);

}

void database() {
    Database db;

    db.createPhoto("pomme", "pomme.jpg");
    db.createVideo("cheval", "cheval.mp4", 26);
    int durations[] = {10, 15};
    db.createFilm("cheval2", "cheval2.mp4", 120, durations, 2);

    auto group1 = db.createGroup("groupe1");

    db.playMedia("pomme");

    db.playMedia("cheval");

    db.deleteMedia("pomme");

    db.deleteGroup("groupe1");
}

void serialisation() {

    Database db;

    db.createPhoto("pomme", "pomme.jpg");
    db.createPhoto("pomme2", "pomme.jpg");
    db.createVideo("cheval", "cheval.mp4", 26);
    int durations[] = {10, 15};
    db.createFilm("cheval2", "cheval2.mp4", 120, durations, 2);
    db.createFilm("cheval33", "cheval2.mp4", 120, durations, 2);
    
    Database db2;
    
    db.saveAll("serialisation.txt");  
    db2.readAll("serialisation.txt");

    db2.findMedia("pomme", std::cout);
    db2.findMedia("cheval", std::cout);

//     Group group1("groupe 1");
//     Group group2("groupe 2");
//     group1.push_back(std::make_shared<Photo>("pomme", "pomme.jpg"));
//     saveAll("serialisation.txt", group1);  // {new Photo("pomme", "pomme.jpg"), new Video("cheval", "cheval.mp4")}
//     readAll("serialisation.txt", group2);
//     group2.printValues(std::cout);
}

int main(int argc, const char* argv[])
{
    serialisation();
}


#endif

