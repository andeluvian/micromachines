#ifndef SPLIT_SCREEN_HPP
#define SPLIT_SCREEN_HPP

#include "race.hpp"

class SplitScreen : public Race
{
public:
    
    SplitScreen(std::string& xmlfile);
    
    virtual ~SplitScreen();
    
    SplitScreen(const SplitScreen&) = delete; // Prevent copying
    
    virtual void drawViewDivider(sf::RenderWindow& window) override;
    
    
private:
    // Note that viewDivider is constructed in the base class constructor
    // but it is not used by the base class.
    void initViewDivider();
    
    
    
};





#endif
