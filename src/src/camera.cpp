#include "camera.hpp"


Camera::Camera(const int& width, const int& height)
: view(sf::Vector2f(width / 2, height / 2), sf::Vector2f(width, height))
{
    // Create view with exactly same size as window
    defaultCenterPoint = {float(width) / 2, float(height) / 2};
    isFollowing = true;
    
    staticView = view;
    
    // Adjust the sizes of the views (half screen)
    leftView = view;
    leftView.setSize(width / 2, height);
    leftView.zoom(1.5); // Zoom out 50% to make player to see larger area
    rightView = view;
    rightView.setSize(width / 2, height);
    rightView.zoom(1.5);
    
    // Define how the view is viewed, i.e., where it is located on the screen.
    // leftView is on the left side and rightView is on the right side.
    leftView.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
    rightView.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));
}


void Camera::setViewToWindow(sf::RenderWindow& window, Views type)
{
    if (type == Views::DEFAULT) {
        window.setView(view);
    }
    else if (type == Views::LEFT) {
        window.setView(leftView);
    }
    else if (type == Views::RIGHT) {
        window.setView(rightView);
    }
    else if (type == Views::STATIC) {
        window.setView(staticView);
    }
    
}

void Camera::followVehicle(Vehicle& vehicle, Views type)
{
    if (!isFollowing)
        return;
    
    if (type == Views::DEFAULT) {
        view.setCenter(vehicle.getPhysics().getPosition().getX(),
                       vehicle.getPhysics().getPosition().getY());
    }
    else if (type == Views::LEFT) {
        leftView.setCenter(vehicle.getPhysics().getPosition().getX(),
                           vehicle.getPhysics().getPosition().getY());
    }
    else if (type == Views::RIGHT) {
        rightView.setCenter(vehicle.getPhysics().getPosition().getX(),
                            vehicle.getPhysics().getPosition().getY());
    }

    
}


void Camera::setDefaultView()
{
    view.setCenter(defaultCenterPoint.x, defaultCenterPoint.y);
}

void Camera::zoomIn(int num)
{
	double p = num / 100.0f;
	double coeff = 1.0 / (1.0 + p);
	view.zoom(coeff); // zoom in 10%
	leftView.zoom(coeff);
	rightView.zoom(coeff);
}
	

void Camera::zoomOut(int num)
{
	double p = num / 100.0f;
	double coeff = 1.0 + p;
	view.zoom(coeff); // zoom in 10%
	leftView.zoom(coeff);
	rightView.zoom(coeff);
}


void Camera::handleEvents(sf::Event &event)
{
    // Define how much the view is moved when the user clicks a keyboard button
    const int step = 40;
    
    if (event.type == sf::Event::KeyPressed) {
        
        // Moving view
        if (event.key.code == sf::Keyboard::Up) {
            view.move(0, -step);
        }
        
        else if (event.key.code == sf::Keyboard::Down) {
            view.move(0, step);
        }
        
        else if (event.key.code == sf::Keyboard::Right) {
            view.move(step, 0);
        }
        
        else if (event.key.code == sf::Keyboard::Left) {
            view.move(-step, 0);
        }
        
        // Zooming view
        else if (event.key.code == sf::Keyboard::M) {
            view.zoom(1.1); // zoom in 10%
            leftView.zoom(1.1);
            rightView.zoom(1.1);
        }
        
        else if (event.key.code == sf::Keyboard::N) {
            view.zoom(1 / 1.1); // zoom out 10%
            leftView.zoom(1 / 1.1);
            rightView.zoom(1 / 1.1);
        }
        
    }
}












