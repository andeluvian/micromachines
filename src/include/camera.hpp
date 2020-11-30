#ifndef CAMERA_HH
#define CAMERA_HH

#include <SFML/Graphics.hpp>

#include "vehicle.hpp"

/*
 * @Author: Miika Karsimus
 *
 * This class is for controlling 2D camera
 *
 * Features to be implemented:
 *
 * - Camera follows car
 * - You can manually control the camera with keyboard (at least while developing the project)
 *   e.g. zooming, moving
 * - Introduce the whole track by moving the camera around the track at the beginning of the race
 *   (if track is large)
 *
 * --------------- VIEW CONTROL BUTTONS --------------------
 *
 * Up (keyboard button): move up
 * Down: move down
 * Right: move right
 * Left: move left
 * M: Zoom in
 * N: Zoom out
 *
 */

class Camera
{
public:
    
    enum Views {
        DEFAULT = 0,
        LEFT = 1,
        RIGHT = 2,
        STATIC = 3
    };
    
    // Pass window width and height as parameter
    Camera(const int& width, const int& height);
    
    // Call this in every loop
    // Param. type is an enum value, e.g. Views::LEFT
    void setViewToWindow(sf::RenderWindow& window, Views type);
    
    /// Control view with keyboard
    void handleEvents(sf::Event& event);
    
    // Set the center point of the view to correspond the position of the car
    void followVehicle(Vehicle& vehicle, Views type);
    
    inline const sf::Vector2f getCenter() const { return view.getCenter(); }
	
	/// Zoom in num %.
	void zoomIn(int num);
	
	/// Zoom out num %.
	void zoomOut(int num);
	
    
    
private:
    sf::View view; // Basic view which follows the vehicle in non-split-screen mode.
    sf::View staticView; // This view will never be moved. Draw static objects in this view.
    // The following two view are for split screen.
    sf::View leftView;
    sf::View rightView;
    
    structures::Point defaultCenterPoint;
    
    /// Set original view.
    void setDefaultView();
    
    /// Tell if the camera is following a car.
    bool isFollowing = false;
    
    /// Tell if the view is in its default position.
    //bool isDefaultView = true;
    
    
};



#endif
