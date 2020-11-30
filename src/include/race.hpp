#ifndef RACE_HH
#define RACE_HH

#include <memory>

#include "vehicle.hpp"
#include "aivehicle.hpp"
#include "track.hpp"
#include "camera.hpp"

/**
 *
 * Author: Miika Karsimus
 *
 * This class represents a single race entity (the logic of the game). The type of the race can time trial, split screen, normal race or practice.
 * SplitScreen and TimeTrial are defined in derived classes. They define a few own functions
 * and attributes specific to that type of race.
 * In addition, they override a couple of virtual Race functions and define their own implemention.
 *
 *
 */

class Race
{
public:
    
    // Race type. Set in constructor.
    enum RaceType {
        TimeTrial = 0,
        SplitScreen = 1,
        NormalRace = 2,
        Practice = 3
    };
    
    /// Pass race type (enum) as parameter.
    Race(std::string& xmlfile);
    virtual ~Race();
    Race(const Race&) = delete;
    
    std::vector<std::shared_ptr<Vehicle>> getVehicles() const;
    std::vector<std::shared_ptr<AIVehicle>> getAIVehicles() const;
    Camera& getCamera();
    sf::Clock& getRaceClock();
    
    /// Add a vehicle to the race.
    virtual void addVehicle(std::shared_ptr<Vehicle> vehicle);
    
    /// Add a AI controlled vehicle to the race.
    void addAIVehicle(std::shared_ptr<AIVehicle> aivehicle);
    
    /// Prepare the race. See cpp file.
    virtual void initialize();
    
    /// Start clock and keyboard event listening.
    void startRace();
    
    /// Check collisions of all the vehicles. Called from a separate thread in Game.
    void checkCollisions();
    
    /// Check weapon, obstacle, finish line etc. hits.
    virtual void checkHits();
    
    /// Handle keyboard events for controlling vehicles and camera.
    void handleEvents(sf::Event& event);
    
    /// Update AI logic and physics.
    void handleAI();
    
    /// Draw everything that moves on the screen along with camera.
    void drawObjects(sf::RenderWindow& window);
    
    /// Draw texts which doesn't move along with camera.
    /// Set camera view to Camera::Views::STATIC before calling this.
    void drawStaticObjects(sf::RenderWindow& window);
    
    /// If player has a weapon, draw the icon next to helmet icon.
    void drawPlayerWeapons(sf::RenderWindow& window);
    
    /// Update vehicle logics. Called from a separate thread in Game.
    virtual void update();
    
	/// Update turbo timing and the acceleration set by said turbo.
	void updateTurbo();

    /// Update bullet positions and check if a bullet hits a vehicle.
    void updateBullets();
    
    /// Update missile positions.
    void updateMissiles();
    
	/// Update sounds. Some sounds are handled by flags, other might not.
	void updateSounds(SoundHandler&);

    /// Update the content of texts on the screen (e.g. lap time, player status, laps).
    void updateTexts();
    
    /// Call this when a player picks up a weapon from the track.
    /// Don't show weapon icon on the track anymore, but show it next to player name
    /// indicating that the player has that weapon.
    void updateWeaponIcons();
    
    /// Create a large text for race countdown in the center of the window.
    void showCountdown(const int count);
    
    /// Update the countdown text based on the elapsed time: 3...2...1...Go!.
    void handleCountdownEvents();
    
    /// Check if a player uses a weapon and use the weapon if a proper keyboard event has occured.
    void handleWeaponEvents(sf::Event& event);
    
    /// Tell if the race is started (countdown has reached ZERO).
    /// When this is false, all keyboard events are ignored (vehicles don't move).
    bool isStarted = false;
    
    /// Tell is the race has ended.
    bool isEnd = false;
    
    bool isSplitScreen() { return splitScreen; }
    
    /// Defined here but function body is empty.
    /// Override in SplitScreen class.
    virtual void drawViewDivider(sf::RenderWindow& window) {}
    
    /// Get racetrack.
    const Track& getTrack() const {return track;}
    
    sf::RectangleShape& getViewDivider() { return viewDivider; }
    
protected:
    int raceType; // use enum value
    int lapsDriven = 1; // Change the name to current lap
    int totalLaps = 5;
    
    sf::Clock raceClock;
    
    // Containers for vehicles and AI vehicles.
    // Pointers are used to utilize polymorphism.
    // shared_ptr is used because racePlaces container shares the ownership of vehicles
    // with this container.
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<std::shared_ptr<AIVehicle>> aivehicles;
    
    Camera camera;
    Track track;
    
    bool splitScreen = false;
    
    sf::Font textFont; // All texts use this.
    sf::Text clockText;
    sf::Text lapsText;
    sf::Text countdownText;
    sf::Text playerStatusText;
    std::vector<sf::Text> playerTexts;
    std::vector<sf::Text> botTexts;
    sf::Sprite flagShape;
    sf::Texture flagTexture;
    sf::Text winnerText;
    
    // Thin black rectange to divide left and right view during split screen game.
    sf::RectangleShape viewDivider;
    
    /// Create a text for race clock in the upper right corner of the window
    /// and for driven laps.
    void createTexts();
    
    // Override these in TimeTrial.
    virtual void createLapTimeText() {}
    virtual void updateLapTimeText() {}
    virtual void drawTimeTrialObjects(sf::RenderWindow& window) {}
    
    /// Declare winner. Stop keyboard event listening (not ready yet)
    void endRace();
    
    void updateWinnerText();
    
    // Player name, HP remaining, helmet icon to be drawn on the screen.
    void createPlayerStatus();

    // Sort racePlaces container based on places in the race. The leader will be the first item.
    void fixPlaces();
    
    std::vector<sf::RectangleShape> helmetIcons;
    sf::Texture helmetTexture;
    std::vector<sf::Color> helmetIconColors = {sf::Color(255, 0, 0), sf::Color(0, 255, 0),
        sf::Color(0, 0, 255), sf::Color(178, 102, 255)
    };
    std::vector<sf::Color> colors = {sf::Color(255, 0, 0), sf::Color(50, 50, 255),
        sf::Color(0, 205, 0), sf::Color(255, 255, 0)};

    
    // Container to hold pointers to Vehicle objects in the specific order.
    // This vector is sorted every time a vehicle passes a checkpoint.
    // Race leader will be the first etc.
    // Also AI vehicles is stored to this container.
    // See Race::fixPlaces().
    std::vector<std::shared_ptr<Vehicle>> racePlaces;


	/// Flags for updateSound()
	bool pickedUpFlag = false;
	bool collisionFlag = false;

};


#endif

