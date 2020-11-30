#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "structures.hpp"

/**
 * This should be moved somewhere else with the xml file.
 * A Simple exception class.
 * Throw if an error occurs during parsing an xml file.
 */
class XMLException : public std::exception {
    
public:
    
    /// Pass error description as parameter.
    
    XMLException(const std::string& msg) : errorMessage(msg) {
    }
    
    // Noexcept (same as throw()) means that this function will never throw an exception.
    
    virtual const char* what() const noexcept {
        // Convert std::string to char*
        return errorMessage.c_str();
    }
    
private:
    std::string errorMessage;
};



class XMLParser
{
public:
    XMLParser(const std::string& filename);
    
    /*
     * Use these functions to access to read track elements.
     * Note that these functions return copies of the objects,
     * thus, you can destroy the XMLParser object as soon as the data is read.
     */
    sf::RectangleShape getTrackFinishLine() const;
    std::vector<sf::RectangleShape> getTrackCheckpoints() const;
    std::vector<sf::RectangleShape> getTrackWalls() const;
    std::vector<structures::Point> getTrackSpawnpoints() const;
    const std::string getWallTextureName() const;
    const std::string getFinishTextureName() const;
    // get weaponPoints
    // get spawnPoints
    
private:
    void readXMLFile();
    const std::string& xmlfile;
    
    // Track elements
    sf::RectangleShape finishLine;
    std::vector<sf::RectangleShape> trackWalls; // Track walls
    std::vector<sf::RectangleShape> trackCheckpoints;
    std::vector<structures::Point> spawnPoints;
    
    std::string finishLineTextureName = "Not implemented";
    std::string wallTextureName;
};



#endif
