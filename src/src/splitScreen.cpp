#include "splitScreen.hpp"


SplitScreen::SplitScreen(std::string& xmlfile) :  Race(xmlfile)
{
    splitScreen = true;
    initViewDivider();
    raceType = RaceType::SplitScreen;
}


SplitScreen::~SplitScreen()
{
    
}


void SplitScreen::initViewDivider()
{
    viewDivider.setFillColor(sf::Color(0, 0, 0));
    viewDivider.setPosition(camera.getCenter().x, camera.getCenter().y);
}


void SplitScreen::drawViewDivider(sf::RenderWindow& window)
{
    window.draw(viewDivider);
}
