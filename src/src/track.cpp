#include <iterator>
#include <iostream>
#include <cstdlib> 
#include <limits>   

#include "track.hpp"
#include "polygon.hpp"
#include "xmlParser.hpp"
#include "obstacle.hpp"
#include "gun.hpp"
#include "missileLauncher.hpp"
#include "turbo.hpp"


Track::Track(const std::string &xmlfile) {

    XMLParser parser(xmlfile);

    // define spawn points
    for (int i = 0; i < 4; i++) {
        structures::Point spawn = {100.0f, 100 + i * 50.0f};
        spawnPoints.push_back(spawn);
    }
    
    // load textures
    textureFinish.setRepeated(true);
    //std::string filename = parser.getFinishTextureName(); not implemented
    std::string filename = "finish.jpg";
    std::string filepath = "../images/" + filename;

    if (!textureFinish.loadFromFile(filepath)) {
        filepath = "../../images/" + filename;
        if (!textureFinish.loadFromFile(filepath)) {
            std::cout << "Error loading texture for finish line!" << std::endl;
            finishLine.setFillColor(sf::Color(0, 0, 255));
        }
    }
    // Get finishLine rectangle from the xml parser.
    finishLine = parser.getTrackFinishLine();
    finishLine.setTexture(&textureFinish);

    // Create 3 oilsplats
    obstacles.insert(obstacles.begin(), 3, Obstacle("notexture"));
    
    // Load oil textures
    if (!textureOil.loadFromFile("../images/oilsplat.png")) {
        if (!textureOil.loadFromFile("../../images/oilsplat.png")) {
            std::cout << "Error loading texture for oil!" << std::endl;
        }
    }
    // Set textures
    for (auto& o : obstacles) {
        o.getShape().setTexture(&textureOil);
    }

    // Define wall texture.
    sf::Color color(255, 255, 255);
    textureWall.setSmooth(true);
    textureWall.setRepeated(true);
    std::string wallTextureName = parser.getWallTextureName();
    std::string texturePath = "../images/" + wallTextureName;
    if (!textureWall.loadFromFile(texturePath)) {
        texturePath = "../../images/" + wallTextureName;
        if (!textureWall.loadFromFile(texturePath)) {
            std::cout << "Error loading texture for walls!" << std::endl;
            color.r = 200;
            color.g = 30;
            color.b = 70;
        }
    }

    // Get walls from the xml parser.
    walls = parser.getTrackWalls();
    
    for (unsigned int i = 0; i < walls.size(); i++) {
        walls[i].setTextureRect(sf::IntRect(0, 0, walls[i].getSize().x * 10, 470));
        walls[i].setTexture(&textureWall);
        walls[i].setFillColor(color);
    }

    // Get checkpoints from the xml parser.
    checkPoints = parser.getTrackCheckpoints();

    // define possible spawn points for weapons.
    weaponPoints = parser.getTrackSpawnpoints();
    
    for (Obstacle &o : obstacles) {
        o.setSpawnPoint(weaponPoints);
    }

    // Testing to add weapons
    
}

void Track::drawTrack(sf::RenderWindow &window) {
    // draw finish line
    window.draw(finishLine);
    // draw walls
    for (sf::RectangleShape wall : walls) {
        window.draw(wall);
    }
   
    // Draw obstacles.
    for (Obstacle obstacle : obstacles) {
        obstacle.drawObstacle(window);
    }
    
    // Draw weapons which are not picked up yet.
    // Must use reference to ptr in iterator, because unique_ptr cannot be copied.
    for (auto &weapon : weapons) {
        weapon->drawWeapon(window);
    }
    
    // Spawn a new weapon
    if (spawnClock.getElapsedTime().asSeconds() > nextSpawnTime) {
        spawnWeapon();
        spawnClock.restart();
		nextSpawnTime = Weapon::getRandomNumber(6, 15);
    }
}

bool Track::isOilSplatHit(const sf::Shape &player) {
    Polygon poly1(player);
    //Polygon poly2(oilsplat);
    for (auto& obstacle : obstacles) {
        Polygon poly2(obstacle.getShape());
        if (poly1.intersects(poly2)) {
            return true;
        }
    }
    return false;
}

int Track::handleWeaponHit(const sf::RectangleShape &vehicleShape) {
    // Called from Race::update()
    int i = 0;
    for (auto &weapon : weapons) {
        if (weapon->getShape().getGlobalBounds().intersects(vehicleShape.getGlobalBounds())) {
            return i;
        }
        i++;
    }
    // No collision
    return -1;
}

std::unique_ptr<Weapon> Track::pickWeapon(const int index) {
    // Test that index is valid.
    if (index < 0 || index >= static_cast<int>(weapons.size())) {
        //return std::make_unique<Weapon>(nullptr);
        throw std::runtime_error("Invalid index");
    }
    std::unique_ptr<Weapon> w = std::move(weapons[index]);
    weapons.erase(weapons.begin() + index);
	for (auto it = reservedSpawnpoints.begin(); it != reservedSpawnpoints.end(); it++) {
		if (*it == index) {
			it = reservedSpawnpoints.erase(it);
			if (it == reservedSpawnpoints.end()) {
				break;
			}
		}
	}
	
    return w;
}

bool Track::isOnFinishLine(const sf::Shape &shape) const {
    Polygon poly1(shape);
    Polygon poly2(finishLine);
    return poly1.intersects(poly2);
}

bool Track::isCheckpointHit(const sf::RectangleShape &rect, const int &index) const {
    // Test that index is valid.
    if (index < 0 || index >= static_cast<int> (getCheckpoints().size())) {
        return false;
    }
    Polygon poly1(rect);
    Polygon poly2(getCheckpoints()[index]);
    return poly1.intersects(poly2);
}

bool Track::isWallHit(const sf::Shape &shape) const {
    Polygon poly1(shape);
    for (sf::RectangleShape wall : walls) {
        Polygon poly2(wall);
        // Test first with bounding boxes. It is redundant to call poly.intersect
        // if even bounding boxes don't collide. This approach reduces CPU load a lot,
        // because calling poly.intersect is a heavy process. 
        if (shape.getGlobalBounds().intersects(wall.getGlobalBounds())) {
            if (poly1.intersects(poly2)) {
                return true;
            }
        }
    }
    return false;
}

void Track::setFinishLine(const sf::RectangleShape& finishLine) {
    this->finishLine = finishLine;
    this->finishLine.setTexture(&textureFinish);
}

void Track::setWalls(const std::vector<sf::RectangleShape>& newWalls) {
    walls.clear();
    for (unsigned int i = 0; i < newWalls.size(); i++) {
        sf::RectangleShape shape(newWalls[i]);
        walls.push_back(shape);
    }
}

void Track::setSpawnpoints(const std::vector<structures::Point> &newPoints) {
    spawnPoints.clear();
    for (unsigned int i = 0; i < newPoints.size(); i++) {
        structures::Point point = {newPoints[i].x, newPoints[i].y};
        spawnPoints.push_back(point);
    }
}

void Track::setCheckpoints(const std::vector<sf::RectangleShape> &newCheckpoints) {
    checkPoints.clear();
    for (unsigned int i = 0; i < newCheckpoints.size(); i++) {
        sf::RectangleShape shape = newCheckpoints[i];
        checkPoints.push_back(shape);
    }
}

void Track::setWeaponPoints(const std::vector<structures::Point> &newPoints) {
    weaponPoints.clear();
    for (unsigned int i = 0; i < newPoints.size(); i++) {
        structures::Point point = {newPoints[i].x, newPoints[i].y};
        weaponPoints.push_back(point);
    }
}

const sf::RectangleShape& Track::getFinishLine() const {
    return finishLine;
}

const Line Track::getCrashedLine(const sf::Shape &shape) {
    Polygon poly1(shape);
    for (sf::RectangleShape wall : walls) {
        Polygon poly2(wall);
        if (poly1.intersects(poly2)) {
            const Line ptr = poly1.getCrashedLine(poly2);
            return Line(ptr.getStart(), ptr.getEnd());
        }
    }
    // Failed to determine line --> return line with NaN-points.
    // Error handling must be performed where function is called.
    structures::Point point = {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    Line line(point, point);
    return line;
}

const std::vector<sf::RectangleShape>& Track::getWalls() const {
    return walls;
}

const std::vector<structures::Point>& Track::getSpawnpoints() const {
    return spawnPoints;
}

const std::vector<sf::RectangleShape>& Track::getCheckpoints() const {
    return checkPoints;
}

const std::vector<structures::Point>& Track::getWeaponPoints() const {
    return weaponPoints;
}

void Track::addWeapon(std::unique_ptr<Weapon> weapon) {
    // set spawn position
    weapon->setSpawnPoint(weaponPoints, reservedSpawnpoints);
    // Use std::move because unique_ptr cannot be copied.
    // The ownership of the weapon is moved to Track.
    weapons.push_back(std::move(weapon));
}

std::vector<std::unique_ptr<Weapon>>& Track::getWeapons() {
    return weapons;
}

void Track::spawnWeapon() {
    // Max 3 weapons on the track
    if (weapons.size() >= 3) {
        return;
    }
    // By taking numbers from wider range and we can
    // change the propabilities for weapons that spawning.
    int num = Weapon::getRandomNumber(0, 100);
    if (num <= 30) {
        addWeapon(std::make_unique<Gun>());
    }
    else if (num >= 90 && missilesSpawned < 2) {
        addWeapon(std::make_unique<MissileLauncher>());
        missilesSpawned++;
    }
    else if (num < 90) {
        addWeapon(std::make_unique<Turbo>());
    } else {
        // if two missiles were already spawned -> spawn turbo
        num = Weapon::getRandomNumber(0, 1);
        if(num == 1)
            addWeapon(std::make_unique<Gun>());
        else
            addWeapon(std::make_unique<Turbo>());
    }
    
}


