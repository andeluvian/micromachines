#include "tinyxml2.hpp"
#include "xmlParser.hpp"


XMLParser::XMLParser(const std::string& filename) : xmlfile(filename)
{
    readXMLFile();
}


sf::RectangleShape XMLParser::getTrackFinishLine() const
{
    return finishLine;
}

std::vector<sf::RectangleShape> XMLParser::getTrackCheckpoints() const
{
    return trackCheckpoints;
}

std::vector<sf::RectangleShape> XMLParser::getTrackWalls() const
{
    return trackWalls;
}

std::vector<structures::Point> XMLParser::getTrackSpawnpoints() const
{
    return spawnPoints;
}

const std::string XMLParser::getWallTextureName() const
{
    return wallTextureName;
}

const std::string XMLParser::getFinishTextureName() const
{
    return finishLineTextureName;
}

void XMLParser::readXMLFile()
{
    using namespace tinyxml2;
    
    // Predeclare this variable here because it must be declared out of try-catch scope.
    
    
    XMLDocument xmlDoc;
    std::string filepath = "../xml/" + xmlfile;
    XMLError eResult = xmlDoc.LoadFile(filepath.c_str()); // c_str converts std::string to char*
    if (eResult != XML_SUCCESS) {
        // With Mac and XCode-IDE I have to use different path to load files.
        filepath = "../../xml/" + xmlfile;
        eResult = xmlDoc.LoadFile(filepath.c_str());
        if (eResult != XML_SUCCESS) {
            throw XMLException("Failed to open XML file.");
        }
    }
    
    // Initialize variables with zero to avoid undefined behaviour
    // if some of the following lines fails...
    int Xpos = 0, Ypos = 0, wdth = 0, hght = 0, Rot = 0;
    
    //Try to open the root
    XMLNode *pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        throw XMLException("Root node not found.");
    }
    
    // define finish line, let's read some data from xml file.
    XMLElement *pFinishline = pRoot->FirstChildElement("Finish_line");
    XMLElement *pElement = pFinishline->FirstChildElement("xpos");
    eResult = pElement->QueryIntText(&Xpos);
    pElement = pFinishline->FirstChildElement("ypos");
    eResult = pElement->QueryIntText(&Ypos);
    pElement = pFinishline->FirstChildElement("width");
    eResult = pElement->QueryIntText(&wdth);
    pElement = pFinishline->FirstChildElement("height");
    eResult = pElement->QueryIntText(&hght);
    
    sf::RectangleShape finish(sf::Vector2f(wdth, hght));
    finish.setPosition(Xpos, Ypos);
    finishLine = finish;
    
    // Define walls.
    
    XMLElement *pWalls = pRoot->FirstChildElement("walls");
    XMLElement *pWall = pWalls->FirstChildElement("wall");
    // Test that the pointers are valid.
    if (pWall == nullptr || pWalls == nullptr) {
        throw XMLException("Wall nodes not found.");
    }
    
    
    // Read texture filename.
    // wallTexture is declared before try-catch scope.
    const char* wallTextureTemp = pWalls->Attribute("texture"); // Returns const char*
    if (wallTextureTemp == nullptr) {
        wallTextureName = ""; // Member attribute
    }
    else {
        wallTextureName = wallTextureTemp;
    }
    
    // Read wall positions.
    while (pWall != nullptr) {
        // Remove eResults because they are unused
        pElement = pWall->FirstChildElement("width");
        eResult = pElement->QueryIntText(&wdth);
        pElement = pWall->FirstChildElement("height");
        eResult = pElement->QueryIntText(&hght);
        pElement = pWall->FirstChildElement("xpos");
        eResult = pElement->QueryIntText(&Xpos);
        pElement = pWall->FirstChildElement("ypos");
        eResult = pElement->QueryIntText(&Ypos);
        pElement = pWall->FirstChildElement("rot");
        eResult = pElement->QueryIntText(&Rot);
        pWall = pWall->NextSiblingElement("wall");
        
        sf::RectangleShape rect1(sf::Vector2f(wdth, hght));
        rect1.setPosition(Xpos, Ypos);
        rect1.setFillColor(sf::Color(200, 30, 70));
        rect1.setRotation(Rot);
        // As soon as the wall position is read, push the wall to the container.
        trackWalls.push_back(rect1);
    }
    
    // Get checkpoints next.
    XMLElement *checkpoints = pRoot->FirstChildElement("checkpoints");
    XMLElement *checkpoint = checkpoints->FirstChildElement("checkpoint");
    if (checkpoints == nullptr || checkpoint == nullptr) {
        throw XMLException("Checkpoint nodes not found.");
    }
    
    // Read checkpoint colors from the xml file.
    int colorR = 0, colorG = 0, colorB = 0;
    checkpoints->QueryIntAttribute("colorR", &colorR);
    checkpoints->QueryIntAttribute("colorG", &colorG);
    checkpoints->QueryIntAttribute("colorB", &colorB);
    
    while (checkpoint != nullptr) {
        pElement = checkpoint->FirstChildElement("width");
        eResult = pElement->QueryIntText(&wdth);
        pElement = checkpoint->FirstChildElement("height");
        eResult = pElement->QueryIntText(&hght);
        pElement = checkpoint->FirstChildElement("xpos");
        eResult = pElement->QueryIntText(&Xpos);
        pElement = checkpoint->FirstChildElement("ypos");
        eResult = pElement->QueryIntText(&Ypos);
        pElement = checkpoint->FirstChildElement("rot");
        eResult = pElement->QueryIntText(&Rot);
        checkpoint = checkpoint->NextSiblingElement("checkpoint");
        
        sf::RectangleShape checkpointRect(sf::Vector2f(wdth, hght));
        checkpointRect.setPosition(Xpos, Ypos);
        checkpointRect.setRotation(Rot);
        checkpointRect.setFillColor(sf::Color(colorR, colorG, colorB));
        trackCheckpoints.push_back(checkpointRect);
    }
    
    // Read spawnpoints for weapons and obstacles
    XMLElement *spawnpoints = pRoot->FirstChildElement("spawnpoints");
    if (spawnpoints == nullptr) {
        throw XMLException("Spawnpoint nodes not found.");
    }
    XMLElement *point = spawnpoints->FirstChildElement("point");
    if (point == nullptr) {
        throw XMLException("Spawnpoint nodes not found.");
    }
    
    int pointX = 0, pointY = 0; // local variables for x and y coordinates
    while (point != nullptr) {
        pElement = point->FirstChildElement("xpos");
        pElement->QueryIntText(&pointX);
        pElement = point->FirstChildElement("ypos");
        pElement->QueryIntText(&pointY);
        point = point->NextSiblingElement("point");
        
        structures::Point p = {float(pointX), float(pointY)};
        spawnPoints.push_back(p);
    }
    
    
    // Now all data is read and saved to local attribute containers.
    
    

    
}
